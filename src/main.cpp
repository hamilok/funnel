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


#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

#include "config.hpp"
#include "server.hpp"

int main(int argc, char** argv)
{
  try
  {
    std::string address = "0.0.0.0";
    std::size_t port = 9500;
    std::size_t thread_cnt = boost::thread::hardware_concurrency();
    std::size_t buffer_size = 65535;
    std::size_t update_int = 900;

    boost::program_options::options_description generic("Generic options");
    generic.add_options()
      ("help,h",    "Produce help message")
      ("version,v", "Print version string");

    boost::program_options::options_description config("Configuration");
    config.add_options()
      ("address,a",   boost::program_options::value<std::string>(&address)->default_value(address),         "IP address. By default, bind to any address.")
      ("port,p",      boost::program_options::value<std::size_t>(&port)->default_value(port),               "Port number.")
      ("threads,t",   boost::program_options::value<std::size_t>(&thread_cnt)->default_value(thread_cnt),   "Threads count. By default, the maximum available.")
      ("buffer,b",    boost::program_options::value<std::size_t>(&buffer_size)->default_value(buffer_size), "Socket receiver buffer size.")
      ("update,u",    boost::program_options::value<std::size_t>(&update_int)->default_value(update_int),   "Update interval in seconds.");

    boost::program_options::options_description hidden("Hidden options");
    hidden.add_options()
      ("zones", boost::program_options::value<std::string>(), "Zones filename")
      ("abonents", boost::program_options::value<std::string>(), "Abonents filename");

    boost::program_options::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    boost::program_options::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    boost::program_options::options_description visible;
    visible.add(generic).add(config);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
      std::cout << cmdline_options << std::endl;
      return 1;
    }
    else if (vm.count("version"))
    {
      std::cout << "funnel version " << FUNNEL_VERSION_MAJOR << "." << FUNNEL_VERSION_MINOR << std::endl;
      return 1;
    }

    /**
     * Start server
     */
    server srv(address, port, thread_cnt, buffer_size, update_int);
    srv.run();

    // Autoloading zones
    if (vm.count("zones"))
    {
      srv.load_zones(vm["zones"].as<std::string>());
      std::cout << std::endl;
    }

    // Autoloading abonents
    if (vm.count("abonents"))
    {
      srv.load_abonents(vm["abonents"].as<std::string>());
      std::cout << std::endl;
    }

    std::string cmd;
    boost::smatch results;
    while (true)
    {
      std::cout << "funnel> ";
      std::getline(std::cin, cmd);

      if (std::cin.eof())
      {
        std::cout << std::endl;
        srv.stop();
        break;
      }

      if (0 == cmd.length())
      {
        continue;
      }

      if (boost::regex_match(cmd, results, boost::regex("(stop|exit|quit)")))
      {
        srv.stop();
        break;
      }
      else if ("help" == cmd)
      {
        std::cout << "zones ( load filename | list | clear )";
        std::cout << std::endl;
        std::cout << "abonents ( load filename | list | clear )";
        std::cout << std::endl;
        std::cout << "statistic ( dump filename | print )";
      }
      else if (boost::regex_match(cmd, results, boost::regex("zones")))
      {
        std::cout << "zones ( load filename | list | clear)";
      }
      else if (boost::regex_match(cmd, results, boost::regex("zones load")))
      {
        std::cout << "zones load filename";
      }
      else if (boost::regex_match(cmd, results, boost::regex("zones load (.+)")))
      {
        srv.load_zones(results[1].str().c_str());
      }
      else if (boost::regex_match(cmd, results, boost::regex("zones list")))
      {
        srv.list_zones();
      }
      else if (boost::regex_match(cmd, results, boost::regex("zones clear")))
      {
        srv.clear_zones();
      }
      else if (boost::regex_match(cmd, results, boost::regex("abonents")))
      {
        std::cout << "abonents ( load filename | list | clear )";
      }
      else if (boost::regex_match(cmd, results, boost::regex("abonents load")))
      {
        std::cout << "abonents load filename";
      }
      else if (boost::regex_match(cmd, results, boost::regex("abonents load (.+)")))
      {
        srv.load_abonents(results[1].str().c_str());
      }
      else if (boost::regex_match(cmd, results, boost::regex("abonents list")))
      {
        srv.list_abonents();
      }
      else if (boost::regex_match(cmd, results, boost::regex("abonents clear")))
      {
        srv.clear_abonents();
      }
      else if (boost::regex_match(cmd, results, boost::regex("statistic")))
      {
        std::cout << "statistic ( dump filename | print )";
      }
      else if (boost::regex_match(cmd, results, boost::regex("statistic dump")))
      {
        std::cout << "statistic dump filename";
      }
      else if (boost::regex_match(cmd, results, boost::regex("statistic dump (.+)")))
      {
        srv.statistic_dump(results[1].str().c_str());
      }
      else if (boost::regex_match(cmd, results, boost::regex("statistic print")))
      {
	srv.statistic_print();
      }
      else
      {
	std::cout << "Unknown command: " << cmd;
      }

      std::cout << std::endl;
    }

    srv.wait ();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
