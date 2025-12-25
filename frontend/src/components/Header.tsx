import React from 'react';
import { BellIcon, UserCircleIcon } from '@heroicons/react/24/outline';

const Header: React.FC = () => {
  return (
    <header className="bg-white shadow-sm">
      <div className="flex items-center justify-between px-6 py-4">
        <div className="flex items-center">
          <h2 className="text-2xl font-bold text-gray-900">Network Packet Analyzer</h2>
        </div>
        
        <div className="flex items-center space-x-4">
          <div className="text-sm text-gray-500">
            <span>Live Capture: </span>
            <span className="text-red-500 font-semibold">Stopped</span>
          </div>
          
          <button className="p-2 text-gray-400 hover:text-gray-600">
            <BellIcon className="w-6 h-6" />
          </button>
          
          <button className="p-2 text-gray-400 hover:text-gray-600">
            <UserCircleIcon className="w-8 h-8" />
          </button>
        </div>
      </div>
    </header>
  );
};

export default Header;