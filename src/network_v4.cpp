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


#include "network_v4.hpp"
#include <boost/lexical_cast.hpp>

network_v4::network_v4 ( unsigned long address, unsigned char cidr )
    : min ( 0 ), max ( 0 )
{
    unsigned long netmask = 0;

    for ( unsigned char i = 0; i < cidr; i++ )
    {
        netmask |= 1 << ( 31 - i );
    }

    min = ntohl ( ( address & netmask ) | ( ~netmask & 1 ) );
    max = ntohl ( ( address & netmask ) | ( ~netmask & 0xfffffffe ) );
}

network_v4 network_v4::from_string( const std::string& network )
{
    std::size_t found = network.find_last_of( "/" );
    if ( found == 0 )
    {
        // FIXME: Throw exception
    }

    boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string( network.substr( 0, found ) );
    unsigned short cidr = boost::lexical_cast<unsigned short>( network.substr( found + 1 ) );

    return network_v4( addr.to_ulong(), cidr );
}
