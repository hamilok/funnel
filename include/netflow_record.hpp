/*
 * netflow_record.hpp
 *
 *  Created on: Jun 17, 2010
 *      Author: hamilok
 */

#ifndef NETFLOW_RECORD_HPP_
#define NETFLOW_RECORD_HPP_

struct nf_record
{
	unsigned int srcaddr; // Source IP address
	unsigned int dstaddr; // Destination IP address
	unsigned int nexthop; // IP address of next hop router
	unsigned short input; // SNMP index of input interface
	unsigned short output; // SNMP index of output interface
	unsigned int dPkts; // Packets in the flow
	unsigned int dOctets; // Total number of Layer 3 bytes in the packets of the flow
	unsigned int first; // SysUptime at start of flow
	unsigned int last; // SysUptime at the time the last packet of the flow was received
	unsigned short srcport; // TCP/UDP source port number or equivalent
	unsigned short dstport; // TCP/UDP destination port number or equivalent
	unsigned char pad1; // Unused (zero) bytes
	unsigned char tcp_flags; // Cumulative OR of TCP flags
	unsigned char prot; // IP protocol type (for example, TCP = 6; UDP = 17)
	unsigned char tos; // IP type of service (ToS)
	unsigned short src_as; // Autonomous system number of the source, either origin or peer
	unsigned short dst_as; // Autonomous system number of the destination, either origin or peer
	unsigned char src_mask; // Source address prefix mask bits
	unsigned char dst_mask; // Destination address prefix mask bits
	unsigned short pad2; // Unused (zero) bytes
};

#endif /* NETFLOW_RECORD_HPP_ */
