#ifndef FUNNEL_STATISTIC_HPP
#define FUNNEL_STATISTIC_HPP

#include <boost/asio.hpp>

struct direction_t
{
    unsigned long incoming;
    unsigned long outgoing;

    direction_t() : incoming( 0 ), outgoing( 0 )
    {}
};

struct statistic_t
{
    unsigned long address;
    direction_t direction[ 4 ];

    statistic_t( unsigned long addr ) : address( addr )
    {}

    friend bool operator<( const unsigned long& lht, const statistic_t& rht )
    {
        return ( lht < rht.address );
    }

    friend bool operator<( const statistic_t& lht, const unsigned long& rht )
    {
        return ( rht < lht.address );
    }

    friend bool operator<( const statistic_t& lht, const statistic_t& rht )
    {
        return ( lht.address < rht.address );
    }

    friend bool operator>=( const statistic_t& lht, const statistic_t& rht )
    {
        return ( lht.address >= rht.address );
    }

    friend std::ostream& operator<<( std::ostream& out, const statistic_t& stat )
    {
        out << boost::asio::ip::address_v4( stat.address ) << ":" << std::endl;
        out << "direct 0: " << stat.direction[ 0 ].incoming << " / " << stat.direction[ 0 ].outgoing << std::endl;
        out << "direct 1: " << stat.direction[ 1 ].incoming << " / " << stat.direction[ 1 ].outgoing << std::endl;
        out << "direct 2: " << stat.direction[ 2 ].incoming << " / " << stat.direction[ 2 ].outgoing << std::endl;
        out << "direct 3: " << stat.direction[ 3 ].incoming << " / " << stat.direction[ 3 ].outgoing << std::endl;
        return out;
    }
};

typedef std::vector< statistic_t > statistic_list_t;
typedef std::vector< statistic_t >::iterator statistic_list_i;

#endif /* FUNNEL_STATISTIC_HPP */
