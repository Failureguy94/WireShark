import React, { useState } from 'react';

interface Packet {
  id: number;
  timestamp: string;
  source: string;
  destination: string;
  protocol: string;
  length: number;
  info: string;
}

interface PacketListProps {
  packets: Packet[];
  onPacketSelect?: (packet: Packet) => void;
}

const PacketList: React.FC<PacketListProps> = ({ packets, onPacketSelect }) => {
  const [selectedPacketId, setSelectedPacketId] = useState<number | null>(null);
  const [filter, setFilter] = useState('');

  const filteredPackets = packets.filter(packet => {
    if (!filter) return true;
    const searchLower = filter.toLowerCase();
    return (
      packet.source.toLowerCase().includes(searchLower) ||
      packet.destination.toLowerCase().includes(searchLower) ||
      packet.protocol.toLowerCase().includes(searchLower) ||
      packet.info.toLowerCase().includes(searchLower)
    );
  });

  const handleRowClick = (packet: Packet) => {
    setSelectedPacketId(packet.id);
    if (onPacketSelect) {
      onPacketSelect(packet);
    }
  };

  const getProtocolColor = (protocol: string) => {
    const colors: { [key: string]: string } = {
      'TCP': 'bg-blue-100 text-blue-800',
      'UDP': 'bg-green-100 text-green-800',
      'ICMP': 'bg-yellow-100 text-yellow-800',
      'HTTP': 'bg-orange-100 text-orange-800',
      'HTTPS/TLS': 'bg-purple-100 text-purple-800',
      'DNS Query': 'bg-cyan-100 text-cyan-800',
      'ARP': 'bg-pink-100 text-pink-800',
    };
    return colors[protocol] || 'bg-gray-100 text-gray-800';
  };

  return (
    <div>
      <div className="flex justify-between items-center p-4 border-b">
        <h3 className="text-lg font-semibold text-gray-800">
          Captured Packets 
          <span className="ml-2 text-sm font-normal text-gray-500">
            ({filteredPackets.length} of {packets.length})
          </span>
        </h3>
        <div className="flex items-center gap-3">
          <input
            type="text"
            placeholder="Filter packets..."
            value={filter}
            onChange={(e) => setFilter(e.target.value)}
            className="px-3 py-2 border border-gray-300 rounded-md text-sm focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent"
          />
        </div>
      </div>
      
      <div className="overflow-x-auto max-h-[500px] overflow-y-auto">
        <table className="min-w-full table-auto">
          <thead className="bg-gray-50 sticky top-0 z-10">
            <tr>
              <th className="px-4 py-3 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider">No.</th>
              <th className="px-4 py-3 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider">Time</th>
              <th className="px-4 py-3 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider">Source</th>
              <th className="px-4 py-3 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider">Destination</th>
              <th className="px-4 py-3 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider">Protocol</th>
              <th className="px-4 py-3 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider">Length</th>
              <th className="px-4 py-3 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider">Info</th>
            </tr>
          </thead>
          <tbody className="bg-white divide-y divide-gray-200">
            {filteredPackets.map((packet) => (
              <tr 
                key={packet.id} 
                onClick={() => handleRowClick(packet)}
                className={`cursor-pointer transition-colors duration-150 ${
                  selectedPacketId === packet.id 
                    ? 'bg-blue-50 border-l-4 border-blue-500' 
                    : 'hover:bg-gray-50'
                }`}
              >
                <td className="px-4 py-2 whitespace-nowrap text-sm text-gray-600 font-medium">{packet.id}</td>
                <td className="px-4 py-2 whitespace-nowrap text-sm text-gray-500 font-mono text-xs">{packet.timestamp}</td>
                <td className="px-4 py-2 whitespace-nowrap text-sm text-gray-700 font-mono">{packet.source}</td>
                <td className="px-4 py-2 whitespace-nowrap text-sm text-gray-700 font-mono">{packet.destination}</td>
                <td className="px-4 py-2 whitespace-nowrap text-sm">
                  <span className={`px-2 py-1 rounded-full text-xs font-semibold ${getProtocolColor(packet.protocol)}`}>
                    {packet.protocol}
                  </span>
                </td>
                <td className="px-4 py-2 whitespace-nowrap text-sm text-gray-500">{packet.length} B</td>
                <td className="px-4 py-2 whitespace-nowrap text-sm text-gray-500 max-w-xs truncate" title={packet.info}>{packet.info}</td>
              </tr>
            ))}
            {filteredPackets.length === 0 && (
              <tr>
                <td colSpan={7} className="px-6 py-12 text-center">
                  <div className="text-gray-400">
                    <svg className="mx-auto h-12 w-12 mb-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                      <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={1} d="M9.75 17L9 20l-1 1h8l-1-1-.75-3M3 13h18M5 17h14a2 2 0 002-2V5a2 2 0 00-2-2H5a2 2 0 00-2 2v10a2 2 0 002 2z" />
                    </svg>
                    <p className="text-lg font-medium">No packets captured</p>
                    <p className="text-sm mt-1">Start the C++ backend with sudo to capture real packets</p>
                    <code className="mt-2 inline-block bg-gray-100 px-3 py-1 rounded text-xs">
                      sudo ./packet_analyzer
                    </code>
                  </div>
                </td>
              </tr>
            )}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default PacketList;