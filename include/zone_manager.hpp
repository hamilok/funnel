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


#ifndef FUNNEL_ZONE_MANAGER_HPP
#define FUNNEL_ZONE_MANAGER_HPP

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "common.hpp"
#include "checksum.hpp"
#include "zone.hpp"

class zone_manager
{
public:
  typedef std::vector<zone> list;
  typedef std::vector<zone>::iterator iter;

public:
  zone_manager();
  virtual ~zone_manager();
  
  void add(const zone& zone);
  void add(unsigned long address, unsigned int cidr, unsigned char code);

  bool load(const std::string& filename);
  iter find(unsigned int address, bool& found);
  void print();
  void clear();
  bool update();
    
private:
  list items;
  boost::filesystem::path filename;
  std::time_t timestamp;
  unsigned char checksum[MD5_DIGEST_LENGTH];
};

#endif // FUNNEL_ZONE_MANAGER_HPP
