#ifndef FUNNEL_ZONE_HPP
#define FUNNEL_ZONE_HPP

class zone
{
public:
    explicit zone ( const unsigned long address, const unsigned char cidr, const unsigned char code )
        : min ( 0 ), max ( 0 ), code ( code )
    {
        unsigned long netmask = 0;

        for ( unsigned char i = 0; i < cidr; i++ )
        {
            netmask |= 1 << ( 31 - i );
        }

        min = ntohl ( ( address & netmask ) | ( ~netmask & 1 ) );
        max = ntohl ( ( address & netmask ) | ( ~netmask & 0xfffffffe ) );
    }

    unsigned long get_min()  { return min;  }
    unsigned long get_max()  { return max;  }
    unsigned char get_code() { return code; }

    friend bool operator < ( const zone& lht, const unsigned long rht )
    {
        return ( lht.max < rht );
    }

    friend bool operator < ( const unsigned long lht, const zone& rht )
    {
        return ( lht < rht.min );
    }

    friend bool operator < ( const zone& lht, const zone& rht )
    {
        return ( lht.max < rht.min );
    }

    friend bool operator > ( const zone& lht, const unsigned long rht )
    {
        return ( lht.min > rht );
    }

    friend bool operator > ( const unsigned long lht, const zone& rht )
    {
        return ( lht > rht.max );
    }

    friend bool operator > ( const zone& lht, const zone& rht )
    {
        return ( lht.min > rht.max );
    }

    friend bool operator == ( const zone& lht, const zone& rht )
    {
        return ( lht.min == rht.min && lht.max == rht.max );
    }

    friend bool operator != ( const zone& lht, const zone& rht )
    {
        return ( lht.min != rht.min || lht.max != rht.max );
    }

    friend std::ostream& operator << ( std::ostream& out, const zone& rht )
    {
        out << rht.code;
        out << " : " << boost::asio::ip::address_v4( htonl ( rht.min ) );
        out << " - " << boost::asio::ip::address_v4( htonl ( rht.max ) );
        return out;
    }

private:
    unsigned long min;
    unsigned long max;
    unsigned char code;
};

#endif /* FUNNEL_ZONE_HPP */
