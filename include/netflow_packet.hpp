#ifndef FUNNEL_NETFLOW_PACKET_HPP
#define FUNNEL_NETFLOW_PACKET_HPP

#include "netflow_header.hpp"
#include "netflow_record.hpp"

struct nf_packet
{
  nf_header hdr;
  nf_record recs[30];
};

#endif /* FUNNEL_NETFLOW_PACKET_HPP */
