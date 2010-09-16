#ifndef FUNNEL_NETWORK_V4_HPP
#define FUNNEL_NETWORK_V4_HPP

#include <boost/asio.hpp>

class network_v4 {
public:
    explicit network_v4( unsigned long addr, unsigned char cidr );

    unsigned long min_host() const;
    unsigned long max_host() const;

    static network_v4 from_string( const std::string& network );

    friend bool operator < ( const network_v4& lht, const unsigned long& rht )
    {
        return ( lht.max_host() < rht );
    }

    friend bool operator < ( const unsigned long& lht, const network_v4& rht )
    {
        return ( lht < rht.min_host() );
    }

    friend bool operator < ( const network_v4& lht, const network_v4& rht )
    {
        return ( lht.max_host() < rht.min_host() );
    }

    friend bool operator > ( const network_v4& lht, const unsigned long& rht )
    {
        return ( lht.min_host() > rht );
    }

    friend bool operator > ( const unsigned long& lht, const network_v4& rht )
    {
        return ( lht > rht.max_host() );
    }

    friend bool operator > ( const network_v4& lht, const network_v4& rht )
    {
        return ( lht.min_host() > rht.max_host() );
    }

    friend bool operator == ( const network_v4& lht, const network_v4& rht )
    {
        return ( lht.min_host() == rht.min_host() && lht.max_host() == rht.max_host() );
    }

    friend bool operator != ( const network_v4& lht, const network_v4& rht )
    {
        return ( lht.min_host() != rht.min_host() || lht.max_host() != rht.max_host() );
    }

    friend std::ostream& operator << ( std::ostream& out, const network_v4& net )
    {
        out << boost::asio::ip::address_v4( net.min_host() );
        out << " - ";
        out << boost::asio::ip::address_v4( net.max_host() );

        return out;
    }

private:
    unsigned long address;
    unsigned long netmask;
};

#endif /* FUNNEL_NETWORK_V4_HPP */
