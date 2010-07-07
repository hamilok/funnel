#ifndef BOOST_ASIO_IP_NETWORK_HPP
#define BOOST_ASIO_IP_NETWORK_HPP

#include <boost/lexical_cast.hpp>

namespace boost {
namespace asio {
namespace ip {

class network {
public:
    network()
        : address(),
          netmask(0)
    {
    }

    network(boost::asio::ip::address addr, unsigned char cidr)
        : address(addr),
          netmask(0)
    {
        int ttt = address.is_v4() ? 31 : 63;

        for (int i = 0; i < cidr; i++)
        {
            netmask |= 1 << (ttt - i);
        }
    }

    boost::asio::ip::address min_host() const
    {
        if (address.is_v4())
        {
            return boost::asio::ip::address_v4( (address.to_v4().to_ulong() & netmask) | (~netmask & 1) );
        }
    }

    boost::asio::ip::address max_host() const
    {
        if (address.is_v4())
        {
            return boost::asio::ip::address_v4( (address.to_v4().to_ulong() & netmask) | (~netmask & 0xfffffffe) );
        }
    }

    static network from_string(const std::string& network)
    {
        std::size_t found = network.find_last_of("/");
        if (found == 0)
        {
            // FIXME: Throw exception
        }

        boost::asio::ip::address addr = boost::asio::ip::address::from_string(network.substr(0, found));
        unsigned short mask = boost::lexical_cast<unsigned short>(network.substr(found + 1));

        if (addr.is_v4())
        {
            return boost::asio::ip::network(addr.to_v4(), mask);
        }
        else
        {
            return boost::asio::ip::network(addr.to_v6(), mask);
        }
    }

    friend std::ostream& operator<< (std::ostream& out, const network& net)
    {
        out << net.min_host() << "-" << net.max_host();
        return out;
    }

private:
    boost::asio::ip::address address;
    unsigned long netmask;
};

bool operator<(const boost::asio::ip::address& lht, const network& rht)
{
    if (lht.is_v4()) {
        return (lht.to_v4() < rht.min_host().to_v4());
    }
}

bool operator<(const network& lht, const boost::asio::ip::address& rht)
{
    if (rht.is_v4()) {
        return (lht.max_host().to_v4() < rht.to_v4());
    }
}

bool operator>(const boost::asio::ip::address& lht, const network& rht)
{
    if (lht.is_v4()) {
        return (lht.to_v4() > rht.max_host().to_v4());
    }
}

bool operator>(const network& lht, const boost::asio::ip::address& rht)
{
    if (rht.is_v4()) {
        return (lht.min_host().to_v4() > rht.to_v4());
    }
}

bool operator==(const boost::asio::ip::address& lht, const network& rht)
{
    if (lht.is_v4()) {
        return (lht.to_v4() >= rht.min_host().to_v4() && lht.to_v4() <= rht.max_host().to_v4());
    }
}

bool operator==(const network& lht, const boost::asio::ip::address& rht)
{
    if (rht.is_v4()) {
        return (lht.min_host().to_v4() <= rht.to_v4() && lht.max_host().to_v4() >= rht.to_v4());
    }
}

bool operator!=(const boost::asio::ip::address& lht, const network& rht)
{
    if (lht.is_v4()) {
        return (lht.to_v4() < rht.min_host().to_v4() || lht.to_v4() > rht.max_host().to_v4());
    }
}

bool operator!=(const network& lht, const boost::asio::ip::address& rht)
{
    if (rht.is_v4()) {
        return (lht.min_host().to_v4() > rht.to_v4() || lht.max_host().to_v4() < rht.to_v4());
    }
}

bool operator<=(const boost::asio::ip::address& lht, const network& rht)
{
    if (lht.is_v4()) {
        return (lht.to_v4() <= rht.min_host().to_v4());
    }
}

bool operator<=(const network& lht, const boost::asio::ip::address& rht)
{
    if (rht.is_v4()) {
        return (lht.max_host().to_v4() <= rht.to_v4());
    }
}

bool operator>=(const boost::asio::ip::address& lht, const network& rht)
{
    if (lht.is_v4()) {
        return (lht.to_v4() >= rht.max_host().to_v4());
    }
}

bool operator>=(const network& lht, const boost::asio::ip::address& rht)
{
    if (rht.is_v4()) {
        return (lht.min_host().to_v4() <= rht.to_v4());
    }
}

}
}
}

#endif /* BOOST_ASIO_IP_NETWORK_HPP */
