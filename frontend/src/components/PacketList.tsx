import React, { useState, useEffect } from 'react';
import { PacketAnalyzerAPI } from '../services/api';

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
  onPacketSelect?: (packet: Packet) => void;
}

const PacketList: React.FC<PacketListProps> = ({ onPacketSelect }) => {
  const [packets, setPackets] = useState<Packet[]>([]);
  const [selectedPacketId, setSelectedPacketId] = useState<number | null>(null);
  const [loading, setLoading] = useState(false);

  const fetchPackets = async () => {
    setLoading(true);
    try {
      const fetchedPackets = await PacketAnalyzerAPI.getPackets();
      setPackets(fetchedPackets);
    } catch (error) {
      console.error('Failed to fetch packets:', error);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchPackets();
    const interval = setInterval(fetchPackets, 2000); // Refresh every 2 seconds
    return () => clearInterval(interval);
  }, []);

  const handleRowClick = (packet: Packet) => {
    setSelectedPacketId(packet.id);
    if (onPacketSelect) {
      onPacketSelect(packet);
    }
  };

  if (loading && packets.length === 0) {
    return (
      <div className="flex justify-center items-center h-64">
        <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-500"></div>
      </div>
    );
  }

  return (
    <div>
      <div className="flex justify-between items-center p-4 border-b">
        <h3 className="text-lg font-semibold">Captured Packets ({packets.length})</h3>
        <button 
          onClick={fetchPackets}
          disabled={loading}
          className="px-4 py-2 bg-blue-500 text-white rounded hover:bg-blue-600 disabled:opacity-50"
        >
          {loading ? 'Refreshing...' : 'Refresh'}
        </button>
      </div>
      
      <div className="overflow-x-auto max-h-96 overflow-y-auto">
        <table className="min-w-full table-auto">
          <thead className="bg-gray-50 sticky top-0">
            <tr>
              <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">No.</th>
              <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Time</th>
              <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Source</th>
              <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Destination</th>
              <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Protocol</th>
              <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Length</th>
              <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Info</th>
            </tr>
          </thead>
          <tbody className="bg-white divide-y divide-gray-200">
            {packets.map((packet) => (
              <tr 
                key={packet.id} 
                onClick={() => handleRowClick(packet)}
                className={`cursor-pointer transition-colors ${
                  selectedPacketId === packet.id 
                    ? 'bg-blue-100 border-blue-500' 
                    : 'hover:bg-gray-50'
                }`}
              >
                <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.id}</td>
                <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500 font-mono">{packet.timestamp}</td>
                <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500 font-mono">{packet.source}</td>
                <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500 font-mono">{packet.destination}</td>
                <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
                  <span className={`px-2 py-1 rounded text-xs font-medium ${
                    packet.protocol === 'TCP' ? 'bg-blue-100 text-blue-800' :
                    packet.protocol === 'UDP' ? 'bg-green-100 text-green-800' :
                    packet.protocol === 'ICMP' ? 'bg-yellow-100 text-yellow-800' :
                    'bg-gray-100 text-gray-800'
                  }`}>
                    {packet.protocol}
                  </span>
                </td>
                <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.length}</td>
                <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500 max-w-xs truncate">{packet.info}</td>
              </tr>
            ))}
            {packets.length === 0 && !loading && (
              <tr>
                <td colSpan={7} className="px-6 py-8 text-center text-gray-500">
                  No packets captured yet. Start packet capture to see data.
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