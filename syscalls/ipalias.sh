#!/bin/bash

# Configuration
INTERFACE="eth0"  # Change this to your interface name
BASE_IP="192.168.100"
START_IP=1
END_IP=200
NETMASK=24

# Add IP aliases
for i in $(seq $START_IP $END_IP); do
    ip addr add ${BASE_IP}.$i/$NETMASK dev $INTERFACE label $INTERFACE:$i
done