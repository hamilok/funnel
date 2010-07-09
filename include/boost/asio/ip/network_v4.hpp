#ifndef BOOST_ASIO_IP_NETWORK_HPP
#define BOOST_ASIO_IP_NETWORK_HPP

#include <boost/lexical_cast.hpp>

namespace boost {
namespace asio {
namespace ip {

class network_v4 {
public:
    network_v4()
        : address( boost::asio::ip::address_v4::any() ),
          netmask( 0 )
    {
    }

    network_v4( boost::asio::ip::address_v4 addr, unsigned char cidr)
        : address( addr ),
          netmask( 0 )
    {
        for (int i = 0; i < cidr; i++)
        {
            netmask |= 1 << (31 - i);
        }
    }

    boost::asio::ip::address_v4 min_host() const
    {
        return boost::asio::ip::address_v4( (address.to_ulong() & netmask) | (~netmask & 1) );
    }

    boost::asio::ip::address_v4 max_host() const
    {
        return boost::asio::ip::address_v4( (address.to_ulong() & netmask) | (~netmask & 0xfffffffe) );
    }

    static network_v4 from_string(const std::string& network)
    {
        std::size_t found = network.find_last_of("/");
        if (found == 0)
        {
            // FIXME: Throw exception
        }

        boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(network.substr(0, found));
        unsigned short mask = boost::lexical_cast<unsigned short>(network.substr(found + 1));

        return boost::asio::ip::network_v4(addr, mask);
    }

    friend bool operator<(const network_v4& lht, const network_v4& rht)
    {
        return (lht.max_host() < rht.min_host());
    }

    friend bool operator>(const network_v4& lht, const network_v4& rht)
    {
        return (lht.min_host() > rht.max_host());
    }

    friend bool operator==(const network_v4& lht, const network_v4& rht)
    {
        return (lht.min_host() == rht.min_host() && lht.max_host() == rht.max_host());
    }

    friend bool operator!=(const network_v4& lht, const network_v4& rht)
    {
        return (lht.min_host() != rht.min_host() || lht.max_host() != rht.max_host());
    }

    friend std::ostream& operator<<(std::ostream& out, const network_v4& net)
    {
        out << net.min_host() << "-" << net.max_host();
        return out;
    }

private:
    boost::asio::ip::address_v4 address;
    unsigned long netmask;
};

/*
bool operator<(const boost::asio::ip::address_v4& lht, const network_v4& rht)
{
    return (lht < rht.min_host());
}
*/

}
}
}

#endif /* BOOST_ASIO_IP_NETWORK_HPP */
