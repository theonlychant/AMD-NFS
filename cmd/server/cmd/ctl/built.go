package main

import (
	"bufio"
	"encoding/json"
	"flag"
	"fmt"
	"net"
	"os"
	"time"
)

const defaultSocket = "/tmp/amdinfer.sock"

type Command struct {
	Cmd    string `json:"cmd"`
	Mode   string `json:"mode,omitempty"`
	Marker string `json:"marker,omitempty"`
	Ts     int64  `json:"ts,omitempty"`
}

func sendCommand(sock string, cmd Command) (string, error) {
	conn, err := net.Dial("unix", sock)
	if err != nil {
		return "", err
	}
	defer conn.Close()

	b, err := json.Marshal(cmd)
	if err != nil {
		return "", err
	}
	// append newline so listener that uses ReadString('\n') can parse
	b = append(b, '\n')
	if _, err := conn.Write(b); err != nil {
		return "", err
	}
	// read a single-line response
	r := bufio.NewReader(conn)
	resp, _ := r.ReadString('\n')
	return resp, nil
}

func main() {
	var sock string
	var cmdName string
	var mode string
	var marker string

	flag.StringVar(&sock, "socket", defaultSocket, "Path to server unix socket")
	flag.StringVar(&cmdName, "cmd", "marker", "Command to send: boost|marker|ping")
	flag.StringVar(&mode, "mode", "", "Mode for boost (e.g., fastmath, fp8, batching)")
	flag.StringVar(&marker, "marker", "", "Marker string to emit for tracing/profiling")
	flag.Parse()

	c := Command{Cmd: cmdName, Mode: mode, Marker: marker, Ts: time.Now().UnixMilli()}
	if c.Cmd == "boost" && c.Mode == "" {
		fmt.Fprintln(os.Stderr, "boost requires --mode (e.g., --mode=fastmath)")
		os.Exit(2)
	}
	if c.Cmd == "marker" && c.Marker == "" {
		fmt.Fprintln(os.Stderr, "marker requires --marker")
		os.Exit(2)
	}

	resp, err := sendCommand(sock, c)
	if err != nil {
		fmt.Fprintf(os.Stderr, "error sending command: %v\n", err)
		os.Exit(1)
	}
	fmt.Printf("response: %s", resp)
}