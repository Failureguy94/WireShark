import React from 'react';
import PacketList from './PacketList';
import PacketDetails from './PacketDetails';
import PacketStatistics from './PacketStatistics';

const Dashboard: React.FC = () => {
  return (
    <div className="container mx-auto px-4 py-6">
      <div className="grid grid-cols-12 gap-6">
        {/* Packet Capture Controls */}
        <div className="col-span-12 bg-white rounded-lg shadow p-4">
          <div className="flex space-x-4">
            <button className="bg-blue-500 text-white px-4 py-2 rounded-md hover:bg-blue-600">
              Start Capture
            </button>
            <button className="bg-red-500 text-white px-4 py-2 rounded-md hover:bg-red-600">
              Stop Capture
            </button>
            <button className="bg-gray-500 text-white px-4 py-2 rounded-md hover:bg-gray-600">
              Clear
            </button>
          </div>
        </div>

        {/* Packet List */}
        <div className="col-span-12 lg:col-span-8 bg-white rounded-lg shadow">
          <PacketList />
        </div>

        {/* Packet Details and Statistics */}
        <div className="col-span-12 lg:col-span-4 space-y-6">
          <PacketDetails />
          <PacketStatistics />
        </div>
      </div>
    </div>
  );
};

export default Dashboard;