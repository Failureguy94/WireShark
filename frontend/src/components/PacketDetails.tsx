import React from 'react';

interface PacketDetailsProps {
  selectedPacket?: {
    id: number;
    timestamp: string;
    source: string;
    destination: string;
    protocol: string;
    length: number;
    info: string;
    payload?: string;
  };
}

const PacketDetails: React.FC<PacketDetailsProps> = ({ selectedPacket }) => {
  return (
    <div className="bg-white rounded-lg shadow p-6">
      <h3 className="text-lg font-semibold text-gray-900 mb-4">Packet Details</h3>
      
      {selectedPacket ? (
        <div className="space-y-4">
          <div>
            <label className="text-sm font-medium text-gray-500">Packet #</label>
            <p className="text-sm text-gray-900">{selectedPacket.id}</p>
          </div>
          
          <div>
            <label className="text-sm font-medium text-gray-500">Timestamp</label>
            <p className="text-sm text-gray-900">{selectedPacket.timestamp}</p>
          </div>
          
          <div>
            <label className="text-sm font-medium text-gray-500">Source</label>
            <p className="text-sm text-gray-900 font-mono">{selectedPacket.source}</p>
          </div>
          
          <div>
            <label className="text-sm font-medium text-gray-500">Destination</label>
            <p className="text-sm text-gray-900 font-mono">{selectedPacket.destination}</p>
          </div>
          
          <div>
            <label className="text-sm font-medium text-gray-500">Protocol</label>
            <p className="text-sm text-gray-900">{selectedPacket.protocol}</p>
          </div>
          
          <div>
            <label className="text-sm font-medium text-gray-500">Length</label>
            <p className="text-sm text-gray-900">{selectedPacket.length} bytes</p>
          </div>
          
          <div>
            <label className="text-sm font-medium text-gray-500">Info</label>
            <p className="text-sm text-gray-900">{selectedPacket.info}</p>
          </div>
          
          {selectedPacket.payload && (
            <div>
              <label className="text-sm font-medium text-gray-500">Payload</label>
              <div className="mt-2 p-3 bg-gray-100 rounded border">
                <pre className="text-xs text-gray-700 whitespace-pre-wrap">
                  {selectedPacket.payload}
                </pre>
              </div>
            </div>
          )}
        </div>
      ) : (
        <p className="text-gray-500 text-center py-8">
          Select a packet to view details
        </p>
      )}
    </div>
  );
};

export default PacketDetails;