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


#ifndef FUNNEL_NETWORK_V4_HPP
#define FUNNEL_NETWORK_V4_HPP

#include <boost/asio.hpp>

class network_v4
{
public:
  network_v4(unsigned long address, unsigned char cidr);

  static network_v4 from_string(const std::string& network);

  friend bool operator<(const network_v4& lht, const unsigned long rht)
  {
    return (lht.max < rht);
  }

  friend bool operator<(const unsigned long lht, const network_v4& rht)
  {
    return (lht < rht.min);
  }

  friend bool operator<(const network_v4& lht, const network_v4& rht)
  {
    return (lht.max < rht.min);
  }

  friend bool operator>(const network_v4& lht, const unsigned long rht)
  {
    return (lht.min > rht);
  }

  friend bool operator>(const unsigned long lht, const network_v4& rht)
  {
    return (lht > rht.max);
  }

  friend bool operator>(const network_v4& lht, const network_v4& rht)
  {
    return (htonl(lht.min) > htonl(rht.max ));
  }

  friend bool operator==(const network_v4& lht, const network_v4& rht)
  {
    return (lht.min == rht.min && lht.max == rht.max);
  }

  friend bool operator!=(const network_v4& lht, const network_v4& rht)
  {
    return (lht.min != rht.min || lht.max != rht.max);
  }

  friend std::ostream& operator<<(std::ostream& out, const network_v4& net)
  {
    out << boost::asio::ip::address_v4(htonl(net.min));
    out << " - ";
    out << boost::asio::ip::address_v4(htonl(net.max));

    return out;
  }

private:
  unsigned long min;
  unsigned long max;
};

#endif /* FUNNEL_NETWORK_V4_HPP */
