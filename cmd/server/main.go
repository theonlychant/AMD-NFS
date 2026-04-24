package main

import (
    "bufio"
    "encoding/json"
    "fmt"
    "net"
    "net/http"
    "os"
    "sync"
    "time"
)

const socketPath = "/tmp/amdinfer.sock"

func apiHandler(w http.ResponseWriter, r *http.Request) {
    w.Header().Set("Content-Type", "text/plain")
    fmt.Fprintln(w, "AMD-Native Inference Server (stub)")
}

type ctlCommand struct {
    Cmd    string `json:"cmd"`
    Mode   string `json:"mode,omitempty"`
    Marker string `json:"marker,omitempty"`
    Ts     int64  `json:"ts,omitempty"`
}

var (
    modeLock sync.Mutex
    modes    = map[string]bool{}
    traceLock sync.Mutex
    traceFile = "/tmp/amdinfer-markers.log"
)

func handleControl(cmd ctlCommand) string {
    switch cmd.Cmd {
    case "boost":
        if cmd.Mode == "" {
            return "error: missing mode\n"
        }
        modeLock.Lock()
        modes[cmd.Mode] = true
        modeLock.Unlock()
        // Return a friendly acknowledgment
        return fmt.Sprintf("boost enabled: %s\n", cmd.Mode)
    case "unboost":
        if cmd.Mode == "" {
            return "error: missing mode\n"
        }
        modeLock.Lock()
        delete(modes, cmd.Mode)
        modeLock.Unlock()
        return fmt.Sprintf("boost disabled: %s\n", cmd.Mode)
    case "marker":
        if cmd.Marker == "" {
            return "error: missing marker\n"
        }
        ts := time.Now().UTC().Format(time.RFC3339Nano)
        line := fmt.Sprintf("%s marker=%s ts=%d\n", ts, cmd.Marker, cmd.Ts)
        traceLock.Lock()
        f, err := os.OpenFile(traceFile, os.O_CREATE|os.O_APPEND|os.O_WRONLY, 0644)
        if err == nil {
            f.WriteString(line)
            f.Close()
        }
        traceLock.Unlock()
        return "marker recorded\n"
    case "ping":
        return "pong\n"
    case "status":
        modeLock.Lock()
        defer modeLock.Unlock()
        // list enabled modes
        out := "modes:"
        for k := range modes {
            out += " " + k
        }
        out += "\n"
        return out
    default:
        return "unknown command\n"
    }
}

func startUnixListener() {
    if _, err := os.Stat(socketPath); err == nil {
        os.Remove(socketPath)
    }
    l, err := net.Listen("unix", socketPath)
    if err != nil {
        fmt.Println("unix listen error:", err)
        return
    }
    defer l.Close()
    for {
        conn, err := l.Accept()
        if err != nil {
            fmt.Println("accept error:", err)
            continue
        }
        go func(c net.Conn){
            defer c.Close()
            r := bufio.NewReader(c)
            line, err := r.ReadString('\n')
            if err != nil {
                return
            }
            // try to parse JSON command
            var cmd ctlCommand
            if err := json.Unmarshal([]byte(line), &cmd); err != nil {
                // not JSON - echo back raw
                fmt.Println("received unix msg (raw):", line)
                c.Write([]byte("ok\n"))
                return
            }
            resp := handleControl(cmd)
            c.Write([]byte(resp))
        }(conn)
    }
}

func main(){
    go startUnixListener()
    http.HandleFunc("/", apiHandler)
    fmt.Println("Listening on :8080 and unix socket", socketPath)
    http.ListenAndServe(":8080", nil)
}
