import React from 'react';
import { ChartBarIcon, CpuChipIcon, ServerIcon, Cog6ToothIcon } from '@heroicons/react/24/outline';

const Sidebar: React.FC = () => {
  return (
    <div className="flex flex-col w-64 bg-gray-800">
      <div className="flex items-center justify-center h-20 shadow-md">
        <h1 className="text-xl font-bold text-white">Mini Wireshark</h1>
      </div>
      <nav className="flex-1 px-2 py-4">
        <div className="space-y-2">
          <button className="flex items-center w-full px-4 py-2 text-sm font-medium text-white bg-gray-900 rounded-md">
            <ChartBarIcon className="w-5 h-5 mr-3" />
            Dashboard
          </button>
          <button className="flex items-center w-full px-4 py-2 text-sm font-medium text-gray-300 hover:text-white hover:bg-gray-700 rounded-md">
            <CpuChipIcon className="w-5 h-5 mr-3" />
            Packet Capture
          </button>
          <button className="flex items-center w-full px-4 py-2 text-sm font-medium text-gray-300 hover:text-white hover:bg-gray-700 rounded-md">
            <ServerIcon className="w-5 h-5 mr-3" />
            Network Interfaces
          </button>
          <button className="flex items-center w-full px-4 py-2 text-sm font-medium text-gray-300 hover:text-white hover:bg-gray-700 rounded-md">
            <Cog6ToothIcon className="w-5 h-5 mr-3" />
            Settings
          </button>
        </div>
      </nav>
      <div className="flex items-center justify-center h-16 bg-gray-700">
        <div className="text-center">
          <p className="text-xs text-gray-400">Status: Idle</p>
        </div>
      </div>
    </div>
  );
};

export default Sidebar;