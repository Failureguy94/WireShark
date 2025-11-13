# C++ Packet Analyzer

## Overview
The C++ Packet Analyzer is a network packet capturing and analysis tool that operates in real-time. It captures, decodes, and analyzes network packets, providing insights into network traffic through a terminal user interface.

## Features
- Live packet capturing from network interfaces.
- Decoding of various network protocols including Ethernet, IP, TCP, and UDP.
- Real-time analysis of captured packets with statistics generation.
- User-friendly terminal interface for displaying packet information.

## Project Structure
```
cpp-packet-analyzer
├── src
│   ├── main.cpp               # Entry point of the application
│   ├── capture
│   │   └── pcap_capture.cpp    # Packet capturing implementation
│   ├── decode
│   │   ├── ethernet.cpp        # Ethernet frame decoding
│   │   ├── ip.cpp              # IP packet decoding
│   │   ├── tcp.cpp             # TCP packet decoding
│   │   └── udp.cpp             # UDP packet decoding
│   ├── analysis
│   │   └── analyzer.cpp        # Packet analysis implementation
│   ├── ui
│   │   └── tui.cpp             # Terminal user interface
│   └── utils
│       └── logger.cpp          # Logging utility
├── include
│   ├── capture
│   │   └── pcap_capture.h       # Header for packet capturing
│   ├── decode
│   │   ├── ethernet.h           # Header for Ethernet decoding
│   │   ├── ip.h                 # Header for IP decoding
│   │   ├── tcp.h                # Header for TCP decoding
│   │   └── udp.h                # Header for UDP decoding
│   ├── analysis
│   │   └── analyzer.h           # Header for packet analysis
│   ├── ui
│   │   └── tui.h                # Header for terminal UI
│   └── utils
│       └── logger.h             # Header for logging utility
├── tests
│   ├── CMakeLists.txt          # Test suite configuration
│   └── analyzer_tests.cpp       # Unit tests for the Analyzer class
├── scripts
│   └── run_capture.sh           # Script to run the capture application
├── examples
│   └── demo_capture.sh          # Example usage script
├── CMakeLists.txt              # CMake configuration file
├── .gitignore                   # Version control ignore file
├── LICENSE                      # Licensing information
└── README.md                    # Project documentation
```

## Installation
1. Clone the repository:
   ```
   git clone <repository-url>
   cd cpp-packet-analyzer
   ```
2. Build the project using CMake:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

## Usage
To run the packet analyzer, execute the following command in the terminal:
```
./cpp-packet-analyzer
```

For more detailed usage instructions, refer to the `examples/demo_capture.sh` script.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request for any enhancements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.
