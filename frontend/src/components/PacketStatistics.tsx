import React, { useState, useEffect } from 'react';
import { Chart as ChartJS, ArcElement, Tooltip, Legend, CategoryScale, LinearScale, BarElement } from 'chart.js';
import { Doughnut } from 'react-chartjs-2';
import { PacketAnalyzerAPI } from '../services/api';

ChartJS.register(ArcElement, Tooltip, Legend, CategoryScale, LinearScale, BarElement);

const PacketStatistics: React.FC = () => {
  const [stats, setStats] = useState({
    totalPackets: 0,
    tcpPackets: 0,
    udpPackets: 0,
    icmpPackets: 0,
    ethernetPackets: 0,
    errorPackets: 0
  });

  useEffect(() => {
    const fetchStats = async () => {
      try {
        const data = await PacketAnalyzerAPI.getStatistics();
        setStats(data);
      } catch (error) {
        console.error('Failed to fetch stats:', error);
      }
    };
    fetchStats();
    const interval = setInterval(fetchStats, 2000);
    return () => clearInterval(interval);
  }, []);

  const protocolData = {
    labels: ['TCP', 'UDP', 'ICMP', 'Other'],
    datasets: [
      {
        data: [
          stats.tcpPackets, 
          stats.udpPackets, 
          stats.icmpPackets, 
          Math.max(0, stats.totalPackets - stats.tcpPackets - stats.udpPackets - stats.icmpPackets)
        ],
        backgroundColor: [
          '#3B82F6',
          '#10B981',
          '#F59E0B',
          '#6B7280'
        ],
        borderWidth: 2,
        borderColor: '#fff'
      }
    ]
  };

  const chartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        position: 'bottom' as const,
        labels: {
          boxWidth: 12,
          padding: 15
        }
      }
    }
  };

  return (
    <div className="bg-white rounded-lg shadow p-6">
      <h3 className="text-lg font-semibold text-gray-800 mb-4">📈 Live Statistics</h3>
      
      {/* Summary Stats */}
      <div className="grid grid-cols-2 gap-3 mb-6">
        <div className="bg-gradient-to-br from-blue-50 to-blue-100 p-4 rounded-lg border border-blue-200">
          <div className="text-2xl font-bold text-blue-600">{stats.totalPackets.toLocaleString()}</div>
          <div className="text-xs text-blue-700 font-medium">Total Packets</div>
        </div>
        
        <div className="bg-gradient-to-br from-green-50 to-green-100 p-4 rounded-lg border border-green-200">
          <div className="text-2xl font-bold text-green-600">{stats.tcpPackets.toLocaleString()}</div>
          <div className="text-xs text-green-700 font-medium">TCP</div>
        </div>
        
        <div className="bg-gradient-to-br from-cyan-50 to-cyan-100 p-4 rounded-lg border border-cyan-200">
          <div className="text-2xl font-bold text-cyan-600">{stats.udpPackets.toLocaleString()}</div>
          <div className="text-xs text-cyan-700 font-medium">UDP</div>
        </div>
        
        <div className="bg-gradient-to-br from-amber-50 to-amber-100 p-4 rounded-lg border border-amber-200">
          <div className="text-2xl font-bold text-amber-600">{stats.icmpPackets.toLocaleString()}</div>
          <div className="text-xs text-amber-700 font-medium">ICMP</div>
        </div>
      </div>

      {/* Protocol Distribution Chart */}
      {stats.totalPackets > 0 && (
        <div className="mb-4">
          <h4 className="text-sm font-medium text-gray-600 mb-3">Protocol Distribution</h4>
          <div style={{ height: '180px' }}>
            <Doughnut data={protocolData} options={chartOptions} />
          </div>
        </div>
      )}
      
      {stats.totalPackets === 0 && (
        <div className="text-center py-8 text-gray-400">
          <p className="text-sm">No data available</p>
          <p className="text-xs mt-1">Start the backend to see statistics</p>
        </div>
      )}
    </div>
  );
};

export default PacketStatistics;