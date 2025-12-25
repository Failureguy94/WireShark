const express = require('express');
const cors = require('cors');
const app = express();

app.use(cors());
app.use(express.json());

// Root route
app.get('/', (req, res) => {
  res.json({ 
    message: 'Mini Wireshark API Server',
    endpoints: [
      'GET /api/packets - Get captured packets',
      'GET /api/stats - Get packet statistics'
    ]
  });
});

// Mock packet data
const generateMockPackets = () => {
  const packets = [];
  const protocols = ['TCP', 'UDP', 'ICMP'];
  const sources = ['192.168.1.100', '192.168.1.101', '192.168.1.102', '10.0.0.5'];
  const destinations = ['8.8.8.8', '1.1.1.1', '172.16.0.1'];
  
  for (let i = 1; i <= 20; i++) {
    packets.push({
      id: i,
      timestamp: new Date().toISOString().slice(0, 19).replace('T', ' '),
      source: sources[Math.floor(Math.random() * sources.length)],
      destination: destinations[Math.floor(Math.random() * destinations.length)],
      protocol: protocols[Math.floor(Math.random() * protocols.length)],
      length: 64 + (i * 10),
      info: protocols[i % 3] === 'TCP' ? 'HTTP GET Request' : 'DNS Query'
    });
  }
  return packets;
};

// API Endpoints
app.get('/api/packets', (req, res) => {
  res.json({ packets: generateMockPackets() });
});

app.get('/api/stats', (req, res) => {
  res.json({
    totalPackets: 20,
    tcpPackets: 12,
    udpPackets: 6,
    icmpPackets: 2,
    ethernetPackets: 20,
    errorPackets: 0
  });
});

const PORT = 8080;
app.listen(PORT, () => {
  console.log(`Mock API server running on http://localhost:${PORT}`);
  console.log(`Endpoints:`);
  console.log(`  GET http://localhost:${PORT}/api/packets`);
  console.log(`  GET http://localhost:${PORT}/api/stats`);
});
