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


#ifndef FUNNEL_BOUNDED_BUFFER_HPP
#define FUNNEL_BOUNDED_BUFFER_HPP

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

template <class T>
class bounded_buffer
{
public:
  typedef boost::circular_buffer<T> container_type;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::value_type value_type;

  explicit bounded_buffer(size_type capacity) : m_unread(0), m_container(capacity) {}

  void push_front(const value_type& item)
  {
    boost::mutex::scoped_lock lock(m_mutex);
    m_not_full.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_full, this));
    m_container.push_front(item);
    ++m_unread;
    lock.unlock();
    m_not_empty.notify_one();
  }

  bool pop_back(value_type* pItem)
  {
    boost::mutex::scoped_lock lock(m_mutex);

    //m_not_empty.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
    bool result = m_not_empty.timed_wait(lock, boost::posix_time::microseconds(500), boost::bind(&bounded_buffer<value_type>::is_not_empty, this));

    if (result)
    {
      *pItem = m_container[--m_unread];
    }
 
    lock.unlock();
    m_not_full.notify_one();
    
    return result;
  }

private:
  bounded_buffer(const bounded_buffer&);            // Disabled copy constructor
  bounded_buffer& operator=(const bounded_buffer&); // Disabled assign operator

  bool is_not_empty() const { return m_unread > 0; }
  bool is_not_full() const { return m_unread < m_container.capacity(); }

  size_type m_unread;
  container_type m_container;

  boost::mutex m_mutex;
  boost::condition m_not_empty;
  boost::condition m_not_full;
};

#endif /* FUNNEL_BOUNDED_BUFFER_HPP */