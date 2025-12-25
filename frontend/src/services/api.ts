import axios from 'axios';

const API_BASE_URL = 'http://localhost:8080/api';

interface Packet {
  id: number;
  timestamp: string;
  source: string;
  destination: string;
  protocol: string;
  length: number;
  info: string;
}

interface Statistics {
  totalPackets: number;
  tcpPackets: number;
  udpPackets: number;
  icmpPackets: number;
  ethernetPackets: number;
  errorPackets: number;
}

export class PacketAnalyzerAPI {
  static async getPackets(): Promise<Packet[]> {
    try {
      const response = await axios.get(`${API_BASE_URL}/packets`);
      return response.data.packets;
    } catch (error) {
      console.error('Error fetching packets:', error);
      return [];
    }
  }

  static async getStatistics(): Promise<Statistics> {
    try {
      const response = await axios.get(`${API_BASE_URL}/stats`);
      return response.data;
    } catch (error) {
      console.error('Error fetching statistics:', error);
      return {
        totalPackets: 0,
        tcpPackets: 0,
        udpPackets: 0,
        icmpPackets: 0,
        ethernetPackets: 0,
        errorPackets: 0
      };
    }
  }
}