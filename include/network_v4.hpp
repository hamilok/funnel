#ifndef FUNNEL_NETWORK_V4_HPP
#define FUNNEL_NETWORK_V4_HPP

#include <boost/asio.hpp>

class network_v4
{
public:
    network_v4 ( unsigned long address, unsigned char cidr );

    static network_v4 from_string( const std::string& network );

    friend bool operator < ( const network_v4& lht, const unsigned long rht )
    {
        return ( lht.max < rht );
    }

    friend bool operator < ( const unsigned long lht, const network_v4& rht )
    {
        return ( lht < rht.min );
    }

    friend bool operator < ( const network_v4& lht, const network_v4& rht )
    {
        return ( lht.max < rht.min );
    }

    friend bool operator > ( const network_v4& lht, const unsigned long rht )
    {
        return ( lht.min > rht );
    }

    friend bool operator > ( const unsigned long lht, const network_v4& rht )
    {
        return ( lht > rht.max );
    }

    friend bool operator > ( const network_v4& lht, const network_v4& rht )
    {
        return ( htonl( lht.min ) > htonl ( rht.max ) );
    }

    friend bool operator == ( const network_v4& lht, const network_v4& rht )
    {
        return ( lht.min == rht.min && lht.max == rht.max );
    }

    friend bool operator != ( const network_v4& lht, const network_v4& rht )
    {
        return ( lht.min != rht.min || lht.max != rht.max );
    }

    friend std::ostream& operator << ( std::ostream& out, const network_v4& net )
    {
        out << boost::asio::ip::address_v4( htonl ( net.min ) );
        out << " - ";
        out << boost::asio::ip::address_v4( htonl ( net.max ) );

        return out;
    }

private:
    unsigned long min;
    unsigned long max;
};

#endif /* FUNNEL_NETWORK_V4_HPP */
