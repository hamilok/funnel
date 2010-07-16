#include "network_v4.hpp"
#include <boost/lexical_cast.hpp>

network_v4::network_v4( unsigned long addr, unsigned char cidr)
	:	address( addr ),
        netmask( 0 )
{
    for ( unsigned char i = 0; i < cidr; i++ )
    {
        netmask |= 1 << ( 31 - i );
    }
}

unsigned long network_v4::min_host() const
{
    return ( ( address & netmask ) | ( ~netmask & 1 ) );
}

unsigned long network_v4::max_host() const
{
    return ( ( address & netmask ) | ( ~netmask & 0xfffffffe ) );
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
