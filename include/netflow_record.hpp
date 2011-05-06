/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Alexander Bonar <hamilok@volia.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef FUNNEL_NETFLOW_RECORD_HPP
#define FUNNEL_NETFLOW_RECORD_HPP

struct nf_record
{
  unsigned int   srcaddr;   // Source IP address
  unsigned int   dstaddr;   // Destination IP address
  unsigned int   nexthop;   // IP address of next hop router
  unsigned short input;     // SNMP index of input interface
  unsigned short output;    // SNMP index of output interface
  unsigned int   dPkts;     // Packets in the flow
  unsigned int   dOctets;   // Total number of Layer 3 bytes in the packets of the flow
  unsigned int   first;     // SysUptime at start of flow
  unsigned int   last;      // SysUptime at the time the last packet of the flow was received
  unsigned short srcport;   // TCP/UDP source port number or equivalent
  unsigned short dstport;   // TCP/UDP destination port number or equivalent
  unsigned char  pad1;      // Unused (zero) bytes
  unsigned char  tcp_flags; // Cumulative OR of TCP flags
  unsigned char  prot;      // IP protocol type (for example, TCP = 6; UDP = 17)
  unsigned char  tos;       // IP type of service (ToS)
  unsigned short src_as;    // Autonomous system number of the source, either origin or peer
  unsigned short dst_as;    // Autonomous system number of the destination, either origin or peer
  unsigned char  src_mask;  // Source address prefix mask bits
  unsigned char  dst_mask;  // Destination address prefix mask bits
  unsigned short pad2;      // Unused (zero) bytes
};

#endif /* FUNNEL_NETFLOW_RECORD_HPP */
