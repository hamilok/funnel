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