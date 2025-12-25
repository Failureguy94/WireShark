import React, { useState } from 'react';

const Sidebar: React.FC = () => {
  const [activeTab, setActiveTab] = useState('dashboard');

  const menuItems = [
    { id: 'dashboard', label: 'Dashboard', icon: '📊' },
    { id: 'capture', label: 'Packet Capture', icon: '🔍' },
    { id: 'interfaces', label: 'Network Interfaces', icon: '🖧' },
    { id: 'settings', label: 'Settings', icon: '⚙️' },
  ];

  return (
    <div className="flex flex-col w-64 bg-gray-800 min-h-screen">
      {/* Logo */}
      <div className="flex items-center justify-center h-20 bg-gray-900">
        <div className="text-center">
          <h1 className="text-xl font-bold text-white">🦈 Mini Wireshark</h1>
          <p className="text-xs text-gray-400">Network Packet Analyzer</p>
        </div>
      </div>
      
      {/* Navigation */}
      <nav className="flex-1 px-4 py-6">
        <ul className="space-y-2">
          {menuItems.map((item) => (
            <li key={item.id}>
              <button
                onClick={() => setActiveTab(item.id)}
                className={`flex items-center w-full px-4 py-3 text-sm font-medium rounded-lg transition-all duration-200 ${
                  activeTab === item.id
                    ? 'bg-blue-600 text-white shadow-lg'
                    : 'text-gray-300 hover:bg-gray-700 hover:text-white'
                }`}
              >
                <span className="mr-3 text-lg">{item.icon}</span>
                {item.label}
              </button>
            </li>
          ))}
        </ul>
        
        {/* Quick Stats */}
        <div className="mt-8 p-4 bg-gray-700 rounded-lg">
          <h3 className="text-xs font-semibold text-gray-400 uppercase mb-3">Quick Info</h3>
          <div className="space-y-2 text-sm">
            <div className="flex justify-between text-gray-300">
              <span>API Port:</span>
              <span className="text-blue-400">8080</span>
            </div>
            <div className="flex justify-between text-gray-300">
              <span>Frontend:</span>
              <span className="text-green-400">3000</span>
            </div>
          </div>
        </div>
      </nav>
      
      {/* Status Footer */}
      <div className="p-4 bg-gray-900">
        <div className="flex items-center justify-between">
          <span className="text-xs text-gray-400">Backend Status</span>
          <div className="flex items-center">
            <div className="w-2 h-2 bg-green-500 rounded-full animate-pulse mr-2"></div>
            <span className="text-xs text-green-400">Live</span>
          </div>
        </div>
        <div className="mt-2 text-xs text-gray-500">
          Run with: <code className="bg-gray-700 px-1 rounded">sudo ./packet_analyzer</code>
        </div>
      </div>
    </div>
  );
};

export default Sidebar;