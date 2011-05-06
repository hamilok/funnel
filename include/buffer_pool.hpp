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


#ifndef FUNNEL_BUFFER_POOL_HPP
#define FUNNEL_BUFFER_POOL_HPP

#include <boost/noncopyable.hpp>

template<class T>
class buffer_pool
{
public:
  explicit buffer_pool(std::size_t pool_size) : next (0)
  {
    buffers.reserve(pool_size);
  }

  T& get_buffer()
  {
    T buf = buffers[next++];
    if (next == buffers.size())
      next = 0;
    return buf;
  }

private:
  std::size_t next;
  std::vector<T> buffers;
};

#endif /* FUNNEL_BUFFER_POOL_HPP */
