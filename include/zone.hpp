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


#ifndef FUNNEL_ZONE_HPP
#define FUNNEL_ZONE_HPP

class zone
{
public:
  explicit zone(const unsigned long address, const unsigned char cidr, const unsigned char code)
    :  min(0), max(0), code (code)
  {
    unsigned long netmask = 0;

    for (unsigned char i = 0; i < cidr; i++)
      netmask |= 1 << (31 - i);

    min = ntohl((address & netmask) | (~netmask & 1));
    max = ntohl((address & netmask) | (~netmask & 0xfffffffe));
  }

  unsigned long get_min()  { return min;  }
  unsigned long get_max()  { return max;  }
  unsigned char get_code() { return code; }

  friend bool operator<(const zone& lht, const unsigned long rht)
  {
    return (lht.max < rht);
  }

  friend bool operator<(const unsigned long lht, const zone& rht)
  {
    return (lht < rht.min);
  }

  friend bool operator<(const zone& lht, const zone& rht)
  {
    return (lht.max < rht.min);
  }

  friend bool operator>(const zone& lht, const unsigned long rht)
  {
    return (lht.min > rht);
  }

  friend bool operator>(const unsigned long lht, const zone& rht)
  {
    return (lht > rht.max);
  }

  friend bool operator>(const zone& lht, const zone& rht)
  {
    return (lht.min > rht.max);
  }

  friend bool operator==(const zone& lht, const zone& rht)
  {
    return (lht.min == rht.min && lht.max == rht.max);
  }

  friend bool operator!=(const zone& lht, const zone& rht)
  {
    return (lht.min != rht.min || lht.max != rht.max);
  }

  friend std::ostream& operator<<(std::ostream& out, const zone& rht)
  {
    out << "0x"  << std::hex << (int) rht.code;
    out << " : " << boost::asio::ip::address_v4(htonl(rht.min));
    out << " - " << boost::asio::ip::address_v4(htonl(rht.max));
    return out;
  }

private:
  unsigned long min;
  unsigned long max;
  unsigned char code;
};

#endif /* FUNNEL_ZONE_HPP */
