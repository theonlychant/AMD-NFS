package main

import (
    "bufio"
    "fmt"
    "net"
    "net/http"
    "os"
)

const socketPath = "/tmp/amdinfer.sock"

func apiHandler(w http.ResponseWriter, r *http.Request) {
    w.Header().Set("Content-Type", "text/plain")
    fmt.Fprintln(w, "AMD-Native Inference Server (stub)")
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
            line, _ := r.ReadString('\n')
            fmt.Println("received unix msg:", line)
            c.Write([]byte("ok\n"))
        }(conn)
    }
}

func main(){
    go startUnixListener()
    http.HandleFunc("/", apiHandler)
    fmt.Println("Listening on :8080 and unix socket", socketPath)
    http.ListenAndServe(":8080", nil)
}
