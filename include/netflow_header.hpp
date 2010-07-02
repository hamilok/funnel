/*
 * netflow_header.hpp
 *
 *  Created on: Jun 17, 2010
 *      Author: hamilok
 */

#ifndef NETFLOW_HEADER_HPP_
#define NETFLOW_HEADER_HPP_

struct nf_header
{
	unsigned short version; // NetFlow export format version number
	unsigned short count; // Number of flows exported in this packet (1-30)
	unsigned int sys_uptime; // Current time in milliseconds since the export device booted
	unsigned int unix_secs; // Current count of seconds since 0000 UTC 1970
	unsigned int unix_nsecs; // Residual nanoseconds since 0000 UTC 1970
	unsigned int flow_sequence; // Sequence counter of total flows seen
	unsigned char engine_type; // Type of flow-switching engine
	unsigned char engine_id; // Slot number of the flow-switching engine
	unsigned short sampling_interval; // First two bits hold the sampling mode; remaining 14 bits hold value of sampling interval
};

#endif /* NETFLOW_HEADER_HPP_ */
