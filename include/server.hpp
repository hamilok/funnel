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


#ifndef FUNNEL_SERVER_HPP
#define FUNNEL_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "checksum.hpp"

#include "common.hpp"
#include "netflow_packet.hpp"
#include "buffer_pool.hpp"

#include "zone_manager.hpp"
#include "abonent.hpp"

#include <boost-1_42/boost/concept_check.hpp>

class server : private boost::noncopyable
{
public:
  explicit server(const std::string& address, std::size_t port, std::size_t thread_cnt, std::size_t buffer_size, std::size_t update_int);

  void run();
  void wait();
  void stop();

  void load_zones(const std::string& filename);
  void list_zones();
  void clear_zones();

  void list_abonent();
  void clear_abonent();

  void statistic_dump();

private:
  void handle_process();

  void handle_update_client();
  void handle_update_zones();
  void handle_update_stat();

  void start_receive();
  void handle_receive(const boost::system::error_code& error, std::size_t bytes_recvd);

private:
  boost::asio::io_service io_service;
  boost::asio::ip::udp::socket socket;
  boost::asio::ip::udp::endpoint sender_endpoint;
  boost::mutex mutex;
  boost::mutex mutex2;

  /**
   * Client
   */
  boost::filesystem::path client_file;
  std::time_t client_timestamp;
  unsigned char client_checksum[MD5_DIGEST_LENGTH];
  std::vector< abonent > client_list;
  boost::asio::deadline_timer client_timer;

  /**
   * Network
   */
  zone_manager zone_mgr;
  boost::asio::deadline_timer zone_timer;

  deque2<nf_packet> packets;

  bool running;
  std::vector < boost::shared_ptr < boost::thread > > threads;

  enum
  {
    max_length = 65535
  };

  union
  {
    nf_packet packet;
    char data[max_length];
  };

  buffer_pool < nf_packet > buff_pool;

  std::size_t buffer_size;

  std::size_t thread_cnt;
  std::size_t update_int;

  std::size_t flows_cnt;
  std::size_t bytes_cnt;
};

#endif /* FUNNEL_SERVER_HPP */