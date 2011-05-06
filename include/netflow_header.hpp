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


#ifndef FUNNEL_NETFLOW_HEADER_HPP
#define FUNNEL_NETFLOW_HEADER_HPP

struct nf_header
{
  unsigned short version;           // NetFlow export format version number
  unsigned short count;             // Number of flows exported in this packet (1-30)
  unsigned int   sys_uptime;        // Current time in milliseconds since the export device booted
  unsigned int   unix_secs;         // Current count of seconds since 0000 UTC 1970
  unsigned int   unix_nsecs;        // Residual nanoseconds since 0000 UTC 1970
  unsigned int   flow_sequence;     // Sequence counter of total flows seen
  unsigned char  engine_type;       // Type of flow-switching engine
  unsigned char  engine_id;         // Slot number of the flow-switching engine
  unsigned short sampling_interval; // First two bits hold the sampling mode; remaining 14 bits hold value of sampling interval

  unsigned short get_version()
  {
    return ntohs(version);
  }

  unsigned short get_count()
  {
    return ntohs(count);
  }
};

#endif /* FUNNEL_NETFLOW_HEADER_HPP */
