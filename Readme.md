# WireShark - Network Packet Analyzer

## Overview
A comprehensive network packet analyzer built with C++ backend and React frontend, inspired by Wireshark. This project captures, analyzes, and visualizes network packets in real-time through a modern web interface.

## Features

### Backend (C++)
- ✅ Real-time packet capturing using libpcap
- ✅ Protocol decoding (Ethernet, IP, TCP, UDP)
- ✅ Packet analysis and statistics
- ✅ HTTP REST API for frontend communication
- ✅ Comprehensive logging system
- ✅ Cross-platform support (Linux, macOS, Windows)

### Frontend (React/TypeScript)
- ✅ Modern, responsive web interface
- ✅ Real-time packet visualization
- ✅ Interactive packet details view
- ✅ Statistics dashboard with charts
- ✅ Protocol-based filtering and highlighting
- ✅ Live data updates

## Project Structure
```
WireShark/
├── setup.sh                    # Automated setup script
├── Readme.md                   # This file
├── WireShark/
│   └── cpp-packet-analyzer/    # C++ Backend
│       ├── src/
│       │   ├── main.cpp        # Application entry point
│       │   ├── capture/        # Packet capture implementation
│       │   ├── decode/         # Protocol decoders
│       │   ├── analysis/       # Packet analysis
│       │   ├── api/           # HTTP server for API
│       │   └── utils/         # Utilities (logging)
│       ├── include/           # Header files
│       ├── tests/             # Unit tests
│       ├── CMakeLists.txt     # Build configuration
│       └── build/             # Build output
└── frontend/                  # React Frontend
    ├── src/
    │   ├── components/        # React components
    │   ├── services/          # API communication
    │   └── App.tsx            # Main application
    ├── package.json           # Node.js dependencies
    └── build/                 # Production build
```

## Quick Start

### Automatic Setup (Recommended)
```bash
# Clone and setup everything
./setup.sh
```

### Manual Setup

#### Prerequisites
- **C++ Compiler**: GCC 7+ or Clang 5+
- **CMake**: 3.10+
- **libpcap**: Development headers
- **Node.js**: 16+
- **npm**: 7+

#### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install cmake build-essential pkg-config libpcap-dev
```

**macOS:**
```bash
brew install cmake libpcap
```

**Windows:**
```bash
# Use MSYS2 or Visual Studio with vcpkg
vcpkg install pcap
```

#### Build Backend
```bash
cd WireShark/cpp-packet-analyzer
mkdir build && cd build
cmake ..
make -j$(nproc)
```

#### Setup Frontend
```bash
cd frontend
npm install
```

## Running the Application

### 1. Start Backend API Server
```bash
cd WireShark/cpp-packet-analyzer/build
sudo ./packet_analyzer
```
*Note: Requires sudo for packet capture privileges*

### 2. Start Frontend Development Server
```bash
cd frontend
npm start
```

### 3. Access the Application
- **Web Interface**: http://localhost:3000
- **Backend API**: http://localhost:8080

## API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/packets` | GET | Get captured packets |
| `/api/stats` | GET | Get capture statistics |

## Usage

1. **Start Capture**: Click "Start Capture" in the web interface
2. **View Packets**: Packets appear in real-time in the main table
3. **Inspect Details**: Click any packet to view detailed information
4. **Monitor Statistics**: View protocol distribution and network statistics
5. **Filter Data**: Use the search and filter options

## Development

### Backend Development
```bash
# Build and run tests
cd WireShark/cpp-packet-analyzer/build
make test

# Enable debug mode
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Frontend Development
```bash
cd frontend

# Run in development mode
npm start

# Build for production
npm run build

# Run tests
npm test
```

## Architecture

### Backend Components
- **PacketSniffer**: Captures packets using libpcap
- **Protocol Decoders**: Parse Ethernet, IP, TCP, UDP headers
- **Analyzer**: Generates statistics and insights
- **HTTPServer**: Provides REST API for frontend
- **Logger**: Comprehensive logging system

### Frontend Components
- **Dashboard**: Main application view
- **PacketList**: Real-time packet table
- **PacketDetails**: Detailed packet inspection
- **PacketStatistics**: Visual statistics and charts
- **API Service**: Backend communication layer

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Troubleshooting

### Common Issues

**Permission Denied (Linux/macOS)**
```bash
# Run with sudo for packet capture
sudo ./packet_analyzer
```

**libpcap Not Found**
```bash
# Ubuntu/Debian
sudo apt-get install libpcap-dev

# macOS
brew install libpcap
```

**Frontend Won't Connect**
- Ensure backend is running on port 8080
- Check if firewall is blocking the connection
- Verify CORS headers in API responses

**Build Failures**
- Ensure all dependencies are installed
- Check CMake version (3.10+ required)
- Verify compiler supports C++17

### Performance Tips
- Use Release build for production: `cmake -DCMAKE_BUILD_TYPE=Release ..`
- Limit packet capture rate if experiencing performance issues
- Close other network monitoring tools to avoid conflicts

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Roadmap

### Completed Features ✅
- [x] Basic packet capture and display
- [x] Protocol decoding (Ethernet, IP, TCP, UDP)
- [x] Web-based interface
- [x] Real-time statistics
- [x] REST API communication

### Planned Features 🚧
- [ ] WebSocket for real-time streaming
- [ ] Advanced packet filtering
- [ ] Packet export functionality
- [ ] Deep packet inspection
- [ ] Network topology visualization
- [ ] Historical data storage
- [ ] Multi-interface support

## Support

For questions, issues, or contributions:
- Create an issue on GitHub
- Check existing documentation
- Review troubleshooting guide above

---

**Status**: Production Ready 🚀
**Last Updated**: December 25, 2025
**Version**: 1.0.0
