import React, { useState, useEffect } from 'react';
import { PacketAnalyzerAPI } from '../services/api';

const Header: React.FC = () => {
  const [stats, setStats] = useState({ totalPackets: 0 });
  const [isLive, setIsLive] = useState(false);

  useEffect(() => {
    const checkStatus = async () => {
      try {
        const data = await PacketAnalyzerAPI.getStatistics();
        setStats(data);
        setIsLive(data.totalPackets > 0);
      } catch {
        setIsLive(false);
      }
    };
    checkStatus();
    const interval = setInterval(checkStatus, 3000);
    return () => clearInterval(interval);
  }, []);

  return (
    <header className="bg-white shadow-sm border-b border-gray-200">
      <div className="flex items-center justify-between px-6 py-4">
        <div className="flex items-center space-x-4">
          <h2 className="text-2xl font-bold text-gray-800">Network Packet Analyzer</h2>
          <span className="text-sm text-gray-500 bg-gray-100 px-2 py-1 rounded">Real-time Capture</span>
        </div>
        
        <div className="flex items-center space-x-6">
          {/* Stats */}
          <div className="hidden md:flex items-center space-x-4 text-sm">
            <div className="text-gray-600">
              Total Packets: <span className="font-semibold text-blue-600">{stats.totalPackets.toLocaleString()}</span>
            </div>
          </div>
          
          {/* Live Status */}
          <div className={`flex items-center px-3 py-1.5 rounded-full text-sm font-medium ${
            isLive ? 'bg-green-100 text-green-700' : 'bg-red-100 text-red-700'
          }`}>
            <div className={`w-2 h-2 rounded-full mr-2 ${
              isLive ? 'bg-green-500 animate-pulse' : 'bg-red-500'
            }`}></div>
            {isLive ? 'Live Capture' : 'Disconnected'}
          </div>
          
          {/* Time */}
          <div className="text-sm text-gray-500">
            {new Date().toLocaleTimeString()}
          </div>
        </div>
      </div>
    </header>
  );
};

export default Header;