#include "network_v4.hpp"

network_v4::network_v4()
	:	address( boost::asio::ip::address_v4::any() ),
        netmask( 0 )
{
}

network_v4::network_v4( boost::asio::ip::address_v4 addr, unsigned char cidr)
	:	address( addr ),
        netmask( 0 )
{
    for (int i = 0; i < cidr; i++)
    {
        netmask |= 1 << (31 - i);
    }
}

boost::asio::ip::address_v4 network_v4::min_host() const
{
    return boost::asio::ip::address_v4( (address.to_ulong() & netmask) | (~netmask & 1) );
}

boost::asio::ip::address_v4 network_v4::max_host() const
{
    return boost::asio::ip::address_v4( (address.to_ulong() & netmask) | (~netmask & 0xfffffffe) );
}

network_v4 network_v4::from_string(const std::string& network)
{
    std::size_t found = network.find_last_of("/");
    if (found == 0)
    {
        // FIXME: Throw exception
    }

    boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(network.substr(0, found));
    unsigned short mask = boost::lexical_cast<unsigned short>(network.substr(found + 1));

    return network_v4(addr, mask);
}


bool operator<(const network_v4& lht, const network_v4& rht)
{
    return (lht.max_host() < rht.min_host());
}

bool operator>(const network_v4& lht, const network_v4& rht)
{
    return (lht.min_host() > rht.max_host());
}

bool operator==(const network_v4& lht, const network_v4& rht)
{
    return (lht.min_host() == rht.min_host() && lht.max_host() == rht.max_host());
}

bool operator!=(const network_v4& lht, const network_v4& rht)
{
    return (lht.min_host() != rht.min_host() || lht.max_host() != rht.max_host());
}

std::ostream& operator<<(std::ostream& out, const network_v4& net)
{
    out << net.min_host() << "-" << net.max_host();
    return out;
}

