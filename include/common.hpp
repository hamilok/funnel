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

#include <iostream>
#include <iterator>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/thread.hpp>

#include "network_v4.hpp"
#include "statistic.hpp"

template< class ForwardIterator, class T >
ForwardIterator binary_search2(ForwardIterator first, ForwardIterator last, const T& value)
{
  first = std::lower_bound(first, last, value);
  if (first != last && !(value < *first))
    return first;

  return last;
}

#endif /* FUNNEL_COMMON_HPP */
