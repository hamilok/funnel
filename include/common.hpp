#ifndef FUNNEL_COMMON_HPP
#define FUNNEL_COMMON_HPP

#include <iostream>
#include <iterator>
#include <algorithm>

#include "network_v4.hpp"

bool operator<(const network_v4& lht, const boost::asio::ip::address_v4& rht)
{
    return (lht.max_host() < rht);
}

bool operator>(const boost::asio::ip::address_v4& lht, const network_v4& rht)
{
    return (lht > rht.max_host());
}

bool operator>(const network_v4& lht, const boost::asio::ip::address_v4& rht)
{
    return (lht.min_host() > rht);
}

bool operator==(const boost::asio::ip::address_v4& lht, const network_v4& rht)
{
    return (lht >= rht.min_host() && lht <= rht.max_host());
}

bool operator==(const network_v4& lht, const boost::asio::ip::address_v4& rht)
{
    return (lht.min_host() <= rht && lht.max_host() >= rht);
}

bool operator!=(const boost::asio::ip::address_v4& lht, const network_v4& rht)
{
    return (lht < rht.min_host() || lht > rht.max_host());
}

bool operator!=(const network_v4& lht, const boost::asio::ip::address_v4& rht)
{
    return (lht.min_host() > rht || lht.max_host() < rht);
}

bool operator<=(const boost::asio::ip::address_v4& lht, const network_v4& rht)
{
    return (lht <= rht.min_host());
}

bool operator<=(const network_v4& lht, const boost::asio::ip::address_v4& rht)
{
    return (lht.max_host() <= rht);
}

bool operator>=(const boost::asio::ip::address_v4& lht, const network_v4& rht)
{
    return (lht >= rht.max_host());
}

bool operator>=(const network_v4& lht, const boost::asio::ip::address_v4& rht)
{
    return (lht.min_host() <= rht);
}

bool operator<(const boost::asio::ip::address_v4& lht, const network_v4& rht)
{
    return (lht < rht.min_host());
}

template <class ForwardIterator, class T>
ForwardIterator binary_search2 ( ForwardIterator first, ForwardIterator last, const T& value )
{
    first = lower_bound(first,last,value);
    if (first!=last && !(value<*first))
        return first;

    return last;
}

#endif /* FUNNEL_COMMON_HPP */
