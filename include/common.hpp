/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Alexander Bonar <hamilok@volia.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
ForwardIterator binary_search2(ForwardIterator first, ForwardIterator last, const T& value)
{
  first = std::lower_bound(first, last, value);
  if (first != last && !(value < *first))
    return first;

  return last;
}

template< class T >
class deque2
{
public:
  deque2() : cnt(0)
  {}

  void add(const T& pkt)
  {
    boost::mutex::scoped_lock lock(mutex);

    data.push_front(pkt);

    cond.notify_all();
    cond.wait(mutex);
  }

  T get ()
  {
    boost::mutex::scoped_lock lock(mutex);

    boost::xtime time;
    boost::xtime_get(&time, boost::TIME_UTC);
    time.sec += 1;

    if (!cond.timed_wait(mutex, time, boost::bind(&deque2::not_empty, this)))
      throw std::exception();

    T pkt = data.back();
    data.pop_back();

    cond.notify_all();

    return pkt;
  }

  bool not_empty()
  {
    return !data.empty();
  }

private:
  boost::mutex mutex;
  boost::condition_variable_any cond;
  std::deque < T > data;
  std::size_t cnt;
};

#endif /* FUNNEL_COMMON_HPP */
