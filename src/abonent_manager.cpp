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


#include "abonent_manager.hpp"

abonent_manager::abonent_manager() : filename("./misc/abonents.dat")
{
}

abonent_manager::~abonent_manager()
{
  clear();
}

bool abonent_manager::load(const std::string& n_filename)
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
    if (!boost::regex_match(record, result, boost::regex("([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})")))
      continue;

    items.push_back(
      abonent(
        boost::asio::ip::address_v4::from_string(result[1]).to_ulong(),
        "EE:EE:EE:EE:EE:EE"
      )
    );
  }

  std::sort(items.begin(), items.end());

  file.close();
}

void abonent_manager::print()
{
  std::size_t cnt = items.size();
  
  std::cout << "+-------------------+-----------------+" << std::endl;
  std::cout << "|    MAC Address    |   IP Address    |" << std::endl;
  std::cout << "+-------------------+-----------------+" << std::endl;
  
  if (0 == cnt)
  {
    std::cout << "|             Empty list              |" << std::endl;
  }
  else
  {
    for (std::size_t i = 0; i < cnt; i++)
    {
      std::cout << "| " << std::setw(17) << items[i].get_mac() << " ";
      std::cout << "| " << std::setw(15) << boost::asio::ip::address_v4(items[i].get_ip()).to_string() << " ";
      std::cout << "|" << std::endl;
    }
  }
  
  std::cout << "+-------------------+-----------------+" << std::endl;
  std::cout << "Total count: " << cnt << std::endl;
}

void abonent_manager::clear()
{
  items.clear();
}

bool abonent_manager::update()
{
  try
  {
    if (0 == boost::filesystem::file_size(filename))
    {
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
        return false;
      }
      
      load(filename.string().c_str());
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

