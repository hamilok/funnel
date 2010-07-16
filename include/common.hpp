#ifndef FUNNEL_COMMON_HPP
#define FUNNEL_COMMON_HPP

#include <iostream>
#include <iterator>
#include <algorithm>

#include "network_v4.hpp"
#include "statistic.hpp"

template< class ForwardIterator, class T >
ForwardIterator binary_search2 ( ForwardIterator first, ForwardIterator last, const T& value )
{
    first = lower_bound( first, last, value );
    if ( first != last && !( value < *first ) )
        return first;

    return last;
}

#endif /* FUNNEL_COMMON_HPP */
