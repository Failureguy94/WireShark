import React, { useState, useEffect, useCallback } from 'react';
import PacketList from './PacketList';
import PacketDetails from './PacketDetails';
import PacketStatistics from './PacketStatistics';
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

const Dashboard: React.FC = () => {
  const [selectedPacket, setSelectedPacket] = useState<Packet | undefined>();
  const [isCapturing, setIsCapturing] = useState(false);
  const [packets, setPackets] = useState<Packet[]>([]);
  const [isConnected, setIsConnected] = useState(false);
  const [autoRefresh, setAutoRefresh] = useState(true);

  const fetchPackets = useCallback(async () => {
    try {
      const fetchedPackets = await PacketAnalyzerAPI.getPackets();
      if (fetchedPackets && fetchedPackets.length > 0) {
        setPackets(fetchedPackets);
        setIsConnected(true);
        setIsCapturing(true);
      }
    } catch (error) {
      setIsConnected(false);
      setIsCapturing(false);
    }
  }, []);

  useEffect(() => {
    fetchPackets();
    let interval: NodeJS.Timeout | null = null;
    if (autoRefresh) {
      interval = setInterval(fetchPackets, 2000);
    }
    return () => {
      if (interval) clearInterval(interval);
    };
  }, [autoRefresh, fetchPackets]);

  const handleStartCapture = () => {
    setAutoRefresh(true);
    setIsCapturing(true);
    fetchPackets();
  };

  const handleStopCapture = () => {
    setAutoRefresh(false);
    setIsCapturing(false);
  };

  const handleClear = () => {
    setPackets([]);
    setSelectedPacket(undefined);
  };

  const handleExport = () => {
    const dataStr = JSON.stringify(packets, null, 2);
    const blob = new Blob([dataStr], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `packet_capture_${new Date().toISOString()}.json`;
    a.click();
    URL.revokeObjectURL(url);
  };

  return (
    <div className="container mx-auto px-4 py-6">
      <div className="grid grid-cols-12 gap-6">
        {/* Packet Capture Controls */}
        <div className="col-span-12 bg-white rounded-lg shadow p-4">
          <div className="flex flex-wrap items-center gap-3">
            <button 
              onClick={handleStartCapture}
              disabled={isCapturing}
              className={`px-4 py-2 rounded-md font-medium transition-all duration-200 ${
                isCapturing 
                  ? 'bg-gray-300 text-gray-500 cursor-not-allowed' 
                  : 'bg-green-500 text-white hover:bg-green-600 active:scale-95'
              }`}
            >
              ▶ Start Capture
            </button>
            <button 
              onClick={handleStopCapture}
              disabled={!isCapturing}
              className={`px-4 py-2 rounded-md font-medium transition-all duration-200 ${
                !isCapturing 
                  ? 'bg-gray-300 text-gray-500 cursor-not-allowed' 
                  : 'bg-red-500 text-white hover:bg-red-600 active:scale-95'
              }`}
            >
              ⏹ Stop Capture
            </button>
            <button 
              onClick={handleClear}
              className="bg-gray-500 text-white px-4 py-2 rounded-md font-medium hover:bg-gray-600 transition-all duration-200 active:scale-95"
            >
              🗑 Clear
            </button>
            <button 
              onClick={handleExport}
              disabled={packets.length === 0}
              className={`px-4 py-2 rounded-md font-medium transition-all duration-200 ${
                packets.length === 0
                  ? 'bg-gray-300 text-gray-500 cursor-not-allowed'
                  : 'bg-blue-500 text-white hover:bg-blue-600 active:scale-95'
              }`}
            >
              📥 Export JSON
            </button>
            <button 
              onClick={fetchPackets}
              className="bg-purple-500 text-white px-4 py-2 rounded-md font-medium hover:bg-purple-600 transition-all duration-200 active:scale-95"
            >
              🔄 Refresh
            </button>
            
            <div className="flex-1"></div>
            
            <div className="flex items-center gap-4">
              <div className="text-sm font-medium">
                Packets: <span className="text-blue-600">{packets.length}</span>
              </div>
              <div className={`flex items-center gap-2 px-3 py-1 rounded-full text-sm font-medium ${
                isConnected ? 'bg-green-100 text-green-700' : 'bg-red-100 text-red-700'
              }`}>
                <div className={`w-2 h-2 rounded-full ${
                  isConnected ? 'bg-green-500 animate-pulse' : 'bg-red-500'
                }`}></div>
                {isConnected ? 'Connected' : 'Disconnected'}
              </div>
              <div className={`flex items-center gap-2 px-3 py-1 rounded-full text-sm font-medium ${
                isCapturing ? 'bg-blue-100 text-blue-700' : 'bg-gray-100 text-gray-600'
              }`}>
                <div className={`w-2 h-2 rounded-full ${
                  isCapturing ? 'bg-blue-500 animate-pulse' : 'bg-gray-400'
                }`}></div>
                {isCapturing ? 'Capturing' : 'Stopped'}
              </div>
            </div>
          </div>
        </div>

        {/* Packet List */}
        <div className="col-span-12 lg:col-span-8 bg-white rounded-lg shadow">
          <PacketList packets={packets} onPacketSelect={setSelectedPacket} />
        </div>

        {/* Packet Details and Statistics */}
        <div className="col-span-12 lg:col-span-4 space-y-6">
          <PacketDetails selectedPacket={selectedPacket} />
          <PacketStatistics />
        </div>
      </div>
    </div>
  );
};

export default Dashboard;