package main

import (
    "log"
    "net"
    _"fmt"
    "golang.org/x/net/icmp"
    "golang.org/x/net/ipv4"
)

func main() {
    conn, err := net.ListenPacket("ip4:icmp", "0.0.0.0")
    if err != nil {
        log.Fatalf("Failed to listen for ICMP packets: %v", err)
    }
    defer conn.Close()

    log.Println("ICMP server is listening...")

    for {
        buffer := make([]byte, 1500)
        n, peer, err := conn.ReadFrom(buffer)
        if err != nil {
            log.Printf("Failed to read packet: %v", err)
            continue
        }

        message, err := icmp.ParseMessage(ipv4.ICMPTypeEcho.Protocol(), buffer[:n])
        if err != nil {
            log.Printf("Failed to parse ICMP message: %v", err)
            continue
        }

        switch message.Body.(type) {
        case *icmp.Echo:
            log.Printf("Received ICMP Echo request from %v", peer)

            // Create ICMP Destination Unreachable (Host Unreachable) reply
            // Include the IPv4 header and the first 8 bytes of the original payload
            ipv4Header := buffer[:20] // assuming no IP options, adjust if needed
            originalPayload := buffer[20:28]

            reply := &icmp.Message{
                Type: ipv4.ICMPTypeDestinationUnreachable,
                Code: 1, // Code 1 means Host Unreachable
                Body: &icmp.DstUnreach{
                    Data: append(ipv4Header, originalPayload...), // Include IPv4 header and first 8 bytes of original payload
                },
            }
            replyData, err := reply.Marshal(nil)
            if err != nil {
                log.Printf("Failed to marshal ICMP reply: %v", err)
                continue
            }

            // Send the reply
            _, err = conn.WriteTo(replyData, peer)
            if err != nil {
                log.Printf("Failed to send ICMP reply: %v", err)
                continue
            }
        default:
            log.Printf("Received non-echo ICMP message from %v", peer)
        }
    }
}

