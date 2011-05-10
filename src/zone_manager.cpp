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


#include "common.hpp"
#include "zone_manager.hpp"

zone_manager::zone_manager() : filename("./misc/networks.dat")
{

}

zone_manager::~zone_manager()
{
  clear();
}

void zone_manager::add(const zone& item)
{
  items.push_back(item);
}

void zone_manager::add(unsigned long address, unsigned int cidr, unsigned char code)
{
  items.push_back(zone(address, cidr, code));
}

bool zone_manager::load(const std::string& n_filename)
{
  boost::filesystem::ifstream file(n_filename, std::ios::binary);
  if (!file.is_open())
  {
    return false;
  }

  clear();

  std::string record;
  boost::smatch result;
  while (!std::getline(file, record).eof())
  {
    if (!boost::regex_match(record, result, boost::regex("([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})/([0-9]{2})")))
      continue;

    add(
      boost::asio::ip::address_v4::from_string(result[1]).to_ulong(),
      atoi(result[2].str().c_str()),
      1
    );
  }
      
  file.close();
  
  return true;
}

zone_manager::iter zone_manager::find(unsigned int address, bool& found)
{
  iter res = binary_search2(items.begin(), items.end(), address);
  
  found = (res != items.end());
  
  return res;
}

void zone_manager::print()
{
  std::size_t cnt = items.size();
  
  std::cout << "+------------+-----------------+-----------------+" << std::endl;
  std::cout << "| Identifier |     Minimal     |     Maximal     |" << std::endl;
  std::cout << "+------------+-----------------+-----------------+" << std::endl;
  
  if (0 == cnt)
  {
    std::cout << "|                   Empty list                   |" << std::endl;
  }
  else
  {
    for (std::size_t i = 0; i < cnt; i++)
    {
      std::cout << "| " << std::setw(10) << (int) items[i].get_code() << " ";
      std::cout << "| " << std::setw(15) << boost::asio::ip::address_v4(htonl(items[i].get_min())).to_string() << " ";
      std::cout << "| " << std::setw(15) << boost::asio::ip::address_v4(htonl(items[i].get_max())).to_string() << " ";
      std::cout << "|"  << std::endl;
    }
  }
  
  std::cout << "+------------+-----------------+-----------------+" << std::endl;
  std::cout << "Total count: " << cnt;
}

void zone_manager::clear()
{
  items.clear();
}

bool zone_manager::update()
{
  try
  {
    if (0 == boost::filesystem::file_size(filename))
    {
      std::cout << "File is empty";
      return false;
    }

    // Calc checksum only modified
    std::time_t current_timestamp = boost::filesystem::last_write_time(filename);
    if (current_timestamp > timestamp)
    {
      timestamp = current_timestamp;

      checksum_md5 md5;
      if (md5.compare_file(filename, checksum))
      {
        std::cout << "No updates";
        return false;
      }
      
      load(filename.string().c_str());

      std::sort(items.begin(), items.end());
    }
  }
  catch (boost::filesystem::filesystem_error& e)
  {
    std::cerr << e.what() << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  
  return true;
}