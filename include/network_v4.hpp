#ifndef FUNNEL_NETWORK_V4_HPP
#define FUNNEL_NETWORK_V4_HPP

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

class network_v4 {
public:
    explicit network_v4();
    explicit network_v4(boost::asio::ip::address_v4 addr, unsigned char cidr);

    boost::asio::ip::address_v4 min_host() const;
    boost::asio::ip::address_v4 max_host() const;

    static network_v4 from_string(const std::string& network);

    friend bool operator<(const network_v4& lht, const network_v4& rht);
    friend bool operator>(const network_v4& lht, const network_v4& rht);
    friend bool operator==(const network_v4& lht, const network_v4& rht);
    friend bool operator!=(const network_v4& lht, const network_v4& rht);

    friend std::ostream& operator<<(std::ostream& out, const network_v4& net);

private:
    boost::asio::ip::address_v4 address;
    unsigned long netmask;
};

#endif /* FUNNEL_NETWORK_V4_HPP */
