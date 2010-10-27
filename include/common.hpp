#ifndef FUNNEL_COMMON_HPP
#define FUNNEL_COMMON_HPP

#include <deque>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <boost/thread.hpp>

#include "network_v4.hpp"
#include "statistic.hpp"
#include <boost-1_42/boost/concept_check.hpp>

template< class ForwardIterator, class T >
ForwardIterator binary_search2 ( ForwardIterator first, ForwardIterator last, const T& value )
{
    first = std::lower_bound( first, last, value );
    if ( first != last && !( value < *first ) )
        return first;

    return last;
}

template< class T >
class deque2
{
public:
    deque2 () : cnt ( 0 )
    {}

    void add ( const T& pkt )
    {
        boost::mutex::scoped_lock lock ( mutex );

        data.push_front ( pkt );

        cond.notify_all ();
        cond.wait ( mutex );
    }

    T get ()
    {
        boost::mutex::scoped_lock lock ( mutex );

        boost::xtime time;
        boost::xtime_get ( &time, boost::TIME_UTC );
        time.sec += 1;

        if ( !cond.timed_wait ( mutex, time, boost::bind ( &deque2::not_empty, this ) ) )
            throw std::exception ();

        T pkt = data.back ();
        data.pop_back ();

        cond.notify_all ();

        return pkt;
    }

    bool not_empty ()
    {
        return !data.empty ();
    }

private:
    boost::mutex mutex;
    boost::condition_variable_any cond;
    std::deque < T > data;
    std::size_t cnt;
};

#endif /* FUNNEL_COMMON_HPP */
