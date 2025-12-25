import React from 'react';
import { Chart as ChartJS, ArcElement, Tooltip, Legend, CategoryScale, LinearScale, BarElement } from 'chart.js';
import { Doughnut, Bar } from 'react-chartjs-2';

ChartJS.register(ArcElement, Tooltip, Legend, CategoryScale, LinearScale, BarElement);

interface PacketStats {
  totalPackets: number;
  protocols: {
    TCP: number;
    UDP: number;
    ICMP: number;
    Other: number;
  };
  topSources: Array<{ ip: string; count: number }>;
}

const PacketStatistics: React.FC = () => {
  // Mock data - replace with real data from API
  const stats: PacketStats = {
    totalPackets: 150,
    protocols: {
      TCP: 89,
      UDP: 45,
      ICMP: 12,
      Other: 4
    },
    topSources: [
      { ip: '192.168.1.100', count: 45 },
      { ip: '10.0.0.5', count: 32 },
      { ip: '172.16.0.10', count: 28 }
    ]
  };

  const protocolData = {
    labels: Object.keys(stats.protocols),
    datasets: [
      {
        data: Object.values(stats.protocols),
        backgroundColor: [
          '#3B82F6',
          '#10B981',
          '#F59E0B',
          '#EF4444'
        ],
        borderWidth: 2,
        borderColor: '#fff'
      }
    ]
  };

  const topSourcesData = {
    labels: stats.topSources.map(source => source.ip),
    datasets: [
      {
        label: 'Packet Count',
        data: stats.topSources.map(source => source.count),
        backgroundColor: '#3B82F6',
        borderColor: '#1E40AF',
        borderWidth: 1
      }
    ]
  };

  const chartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        position: 'bottom' as const
      }
    }
  };

  return (
    <div className="bg-white rounded-lg shadow p-6">
      <h3 className="text-lg font-semibold text-gray-900 mb-6">Statistics</h3>
      
      {/* Summary Stats */}
      <div className="grid grid-cols-2 gap-4 mb-6">
        <div className="bg-blue-50 p-4 rounded-lg">
          <div className="text-2xl font-bold text-blue-600">{stats.totalPackets}</div>
          <div className="text-sm text-blue-800">Total Packets</div>
        </div>
        
        <div className="bg-green-50 p-4 rounded-lg">
          <div className="text-2xl font-bold text-green-600">{stats.protocols.TCP}</div>
          <div className="text-sm text-green-800">TCP Packets</div>
        </div>
      </div>

      {/* Protocol Distribution */}
      <div className="mb-6">
        <h4 className="text-md font-medium text-gray-700 mb-3">Protocol Distribution</h4>
        <div style={{ height: '200px' }}>
          <Doughnut data={protocolData} options={chartOptions} />
        </div>
      </div>

      {/* Top Sources */}
      <div>
        <h4 className="text-md font-medium text-gray-700 mb-3">Top Sources</h4>
        <div style={{ height: '150px' }}>
          <Bar data={topSourcesData} options={chartOptions} />
        </div>
      </div>
    </div>
  );
};

export default PacketStatistics;