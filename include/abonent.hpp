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


#ifndef FUNNEL_ABONENT_HPP
#define FUNNEL_ABONENT_HPP

#include "statistic.hpp"

class abonent
{
public:
  explicit abonent(const unsigned long ip_addr, const std::string& mac_addr)
    :  ip(ntohl(ip_addr)), mac(mac_addr)
  {
  }

  friend bool operator<( const unsigned long lht, const abonent& rht)
  {
    return (lht < rht.ip);
  }

  friend bool operator<(const abonent& lht, const unsigned long rht)
  {
    return (lht.ip < rht);
  }

  friend bool operator<(const abonent& lht, const abonent& rht)
  {
    return (lht.ip < rht.ip);
  }

  friend bool operator>(const unsigned long lht, const abonent& rht)
  {
    return (lht > rht.ip);
  }

  friend bool operator>(const abonent& lht, const unsigned long rht)
  {
    return (lht.ip > rht);
  }

  friend bool operator>(const abonent& lht, const abonent& rht)
  {
    return (lht.ip > rht.ip);
  }

  friend bool operator==(const abonent& lht, const abonent& rht)
  {
    return (lht.ip == rht.ip);
  }

  friend bool operator!=(const abonent& lht, const abonent& rht)
  {
    return (lht.ip != rht.ip);
  }

  // Stdout
  friend std::ostream& operator<<(std::ostream& out, const abonent& rht)
  {
    out << boost::asio::ip::address_v4(htonl(rht.ip)) << " " << rht.mac;
    return out;
  }

  // Fileout
  friend std::ofstream& operator<<(std::ofstream& out, const abonent& rht)
  {
    for (std::size_t i = 0; i < 3; i++)
    {
      out << boost::asio::ip::address_v4(htonl(rht.ip));
      out << "|";
      out <<  i;
      out << "|";
      out << htonl(rht.dir[i].incoming);
      out << "|";
      out << htonl(rht.dir[i].outgoing);
      out << std::endl;
    }

    return out;
  }
  
  unsigned long get_ip() const
  {
    return htonl(ip);
  }
  
  std::string get_mac() const
  {
    return mac;
  }

public:
  direction dir[3];

private:
  unsigned long ip;
  std::string mac;
};

#endif /* FUNNEL_ABONENT_HPP */
