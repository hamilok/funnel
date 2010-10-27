#ifndef FUNNEL_ABONENT_HPP
#define FUNNEL_ABONENT_HPP

#include "statistic.hpp"

class abonent
{
public:
    explicit abonent ( const unsigned long ip_addr, const std::string& mac_addr )
        : ip ( ntohl ( ip_addr ) ), mac ( mac_addr )
    {
    }

    friend bool operator < ( const unsigned long lht, const abonent& rht )
    {
        return ( lht < rht.ip );
    }

    friend bool operator < ( const abonent& lht, const unsigned long rht )
    {
        return ( lht.ip < rht );
    }

    friend bool operator < ( const abonent& lht, const abonent& rht )
    {
        return ( lht.ip < rht.ip );
    }

    friend bool operator > ( const unsigned long lht, const abonent& rht )
    {
        return ( lht > rht.ip );
    }

    friend bool operator > ( const abonent& lht, const unsigned long rht )
    {
        return ( lht.ip > rht );
    }

    friend bool operator > ( const abonent& lht, const abonent& rht )
    {
        return ( lht.ip > rht.ip );
    }

    friend bool operator == ( const abonent& lht, const abonent& rht )
    {
        return ( lht.ip == rht.ip );
    }

    friend bool operator != ( const abonent& lht, const abonent& rht )
    {
        return ( lht.ip != rht.ip );
    }

    friend std::ostream& operator << ( std::ostream& out, const abonent& rht )
    {
        out << boost::asio::ip::address_v4 ( htonl ( rht.ip ) ) << " " << rht.mac;
        return out;
    }

    friend std::ofstream& operator << ( std::ofstream& out, const abonent& rht )
    {
        for ( std::size_t i = 0; i < 3; i++ )
            out << boost::asio::ip::address_v4 ( htonl ( rht.ip ) ) << "|" <<  i << "|" << htonl ( rht.dir[ i ].incoming ) << "|" << htonl ( rht.dir[ i ].outgoing ) << std::endl;
        return out;
    }

public:
    direction dir[3];

private:
    unsigned long ip;
    std::string mac;
};

#endif /* FUNNEL_ABONENT_HPP */
