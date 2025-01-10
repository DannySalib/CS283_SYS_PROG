#!/bin/bash
INTERFACE="eth0"
for i in {1..254}; do
    sudo ip addr add 192.168.100.$i/24 dev $INTERFACE
done
