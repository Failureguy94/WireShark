#!/bin/bash

# This script demonstrates how to use the packet capture tool.

# Check if the user has provided a network interface
if [ -z "$1" ]; then
  echo "Usage: $0 <network_interface>"
  exit 1
fi

INTERFACE=$1

# Start the packet capture
echo "Starting packet capture on interface: $INTERFACE"
./cpp-packet-analyzer $INTERFACE

# Note: Ensure that the cpp-packet-analyzer binary is built and available in the current directory.