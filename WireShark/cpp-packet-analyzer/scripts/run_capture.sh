#!/bin/bash

# Check if the user has provided a network interface
if [ -z "$1" ]; then
  echo "Usage: $0 <network_interface>"
  exit 1
fi

INTERFACE=$1

# Run the packet capture application
echo "Starting packet capture on interface: $INTERFACE"
./build/cpp-packet-analyzer --interface $INTERFACE