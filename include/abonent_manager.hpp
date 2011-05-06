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


#ifndef FUNNEL_ABONENT_MANAGER_H
#define FUNNEL_ABONENT_MANAGER_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "common.hpp"
#include "checksum.hpp"
#include "abonent.hpp"

class abonent_manager
{
public:
  abonent_manager();
  virtual ~abonent_manager();

  bool load(const std::string& n_filename);
  void print();
  void clear();
  bool update();
  
private:
  std::vector<abonent> items;
  boost::filesystem::path filename;
  unsigned char checksum[MD5_DIGEST_LENGTH];
  std::time_t timestamp;
};

#endif // FUNNEL_ABONENT_MANAGER_H
