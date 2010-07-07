#ifndef FUNNEL_COMMON_HPP
#define FUNNEL_COMMON_HPP

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <signal.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <boost/lexical_cast.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include "subscriber.hpp"
#include "netflow_header.hpp"
#include "netflow_record.hpp"

#include "boost/asio/ip/network_v4.hpp"

template <class ForwardIterator, class T>
ForwardIterator binary_search2 ( ForwardIterator first, ForwardIterator last, const T& value )
{
    first = lower_bound(first,last,value);
    if (first!=last && !(value<*first))
        return first;

    return last;
}

#endif /* FUNNEL_COMMON_HPP */
