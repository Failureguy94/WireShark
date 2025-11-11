import React from 'react';

interface Packet {
  id: number;
  timestamp: string;
  source: string;
  destination: string;
  protocol: string;
  length: number;
  info: string;
}

const PacketList: React.FC = () => {
  const [packets, setPackets] = React.useState<Packet[]>([]);

  return (
    <div className="overflow-x-auto">
      <table className="min-w-full table-auto">
        <thead className="bg-gray-50">
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
            <tr key={packet.id} className="hover:bg-gray-50">
              <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.id}</td>
              <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.timestamp}</td>
              <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.source}</td>
              <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.destination}</td>
              <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.protocol}</td>
              <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.length}</td>
              <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{packet.info}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default PacketList;