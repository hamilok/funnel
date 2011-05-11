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
#include "server.hpp"

server::server(const std::string& address, std::size_t port, std::size_t thread_cnt, std::size_t buffer_size, std::size_t update_int)
  :  socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(address), port)),
     zone_timer(io_service, boost::posix_time::seconds(update_int)),
     abonent_timer(io_service, boost::posix_time::seconds(update_int)),
     thread_cnt(thread_cnt),
     update_int(update_int),
     buffer_size(buffer_size),
     flows_cnt(0),
     bytes_cnt(0),
     running (true),
     packets(buffer_size)
{
  /// Set receive buffer size for socket
  boost::asio::socket_base::receive_buffer_size option(buffer_size);
  socket.set_option(option);
  
  // Check receive buffer size for socket
  socket.get_option(option);
  if (buffer_size > option.value())
  {
    std::cout << "Socket receive buffer size: " <<  option.value() << std::endl;
  }

  // Start
  start_receive();

  zone_timer.async_wait(boost::bind(&server::handle_update_zones, this));
  abonent_timer.async_wait(boost::bind(&server::handle_update_abonents, this));
}

void server::run()
{
  for (std::size_t i = 0; i < thread_cnt; ++i)
  {
    boost::shared_ptr<boost::thread> thread(
      new boost::thread(
        boost::bind(
          &boost::asio::io_service::run,
          &io_service
        )
      )
    );
    threads.push_back(thread);
  }

  boost::shared_ptr<boost::thread> thread(
    new boost::thread(
      boost::bind(
        &server::handle_process,
        this
      )
    )
  );
  threads.push_back(thread);
}

void server::wait()
{
  for (std::size_t i = 0; i < threads.size(); ++i)
    threads[i]->join();
}

void server::stop()
{
  running = false;
  io_service.stop();
}

void server::load_zones(const std::string& filename)
{
  std::cout << "Loading zones from \"" << filename << "\": ";
  
  if (zone_mgr.load(filename))
    std::cout << "OK";
  else
    std::cout << "Failed";
}

void server::list_zones()
{
  zone_mgr.print();
}

void server::clear_zones()
{
  zone_mgr.clear();
  std::cout << "Zones cleared";
}

void server::load_abonents(const std::string& filename)
{
  std::cout << "Loading abonents from \"" << filename << "\": ";
  
  if (abonent_mgr.load(filename))
    std::cout << "OK";
  else
    std::cout << "Failed";
}

void server::list_abonents()
{
  abonent_mgr.print();
}

void server::clear_abonents()
{
  abonent_mgr.clear();
  std::cout << "Abonents cleared";
}

void server::statistic_dump(const std::string& filename)
{
  abonent_mgr.dump(filename);
}

void server::statistic_print()
{
  std::cout << "flow count: " << flows_cnt;
  std::cout << std::endl;
  std::cout << "bytes count: " << bytes_cnt;
}

void server::handle_update_stats()
{
}

void server::handle_update_zones()
{
  zone_mgr.update();

  zone_timer.expires_from_now(boost::posix_time::seconds(update_int));
  zone_timer.async_wait(boost::bind(&server::handle_update_zones, this));
}

void server::handle_update_abonents()
{
  abonent_mgr.update();

  abonent_timer.expires_from_now(boost::posix_time::seconds(update_int));
  abonent_timer.async_wait(boost::bind(&server::handle_update_abonents, this));
}

void server::start_receive()
{
  socket.async_receive_from(
    boost::asio::buffer(&packet, 65535),
    sender_endpoint,
    boost::bind (
      &server::handle_receive,
      this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}

void server::handle_receive(const boost::system::error_code& error, std::size_t bytes_recvd)
{
  //std::cout << boost::this_thread::get_id() << ": ";

  // Is error
  if (error || bytes_recvd == 0)
  {
    std::cout << "Unknown error" << std::endl;
    return start_receive();
  }

  // Data is not netflow version 5
  if (packet.hdr.get_version() != 5)
  {
    std::cout << "Error: received unknwon packet version" << std::endl;    
    return start_receive();
  }

  // Push packet
  packets.push_front(packet);

  start_receive();
}

void server::handle_process()
{
  nf_packet pkt;
  bool found = false;
  zone_manager::iter zone;
  abonent_manager::iter abonent;
  unsigned char zone_code = 0;

  while (running)
  {
    // Pop packet
    packets.pop_back(&pkt);
    
    flows_cnt += 1;

    // Each per flow
    for (std::size_t i = 0; i < pkt.hdr.get_count(); i++)
    {
      // Reset zone
      zone_code = 0;
      bytes_cnt += pkt.recs[i].dOctets;

      abonent = abonent_mgr.find(pkt.recs[i].srcaddr, found);
      if (found)
      {
        zone = zone_mgr.find(pkt.recs[i].dstaddr, found);
        if (found)
        {
          zone_code = (*zone).get_code();
        }

        (*abonent).dir[zone_code].outgoing += pkt.recs[i].dOctets;
      }
      else
      {
        abonent = abonent_mgr.find(pkt.recs[i].dstaddr, found);
        if (found)
        {
          zone = zone_mgr.find(pkt.recs[i].srcaddr, found);
          if (found)
          {
            zone_code = (*zone).get_code();
          }

          (*abonent).dir[zone_code].incoming += pkt.recs[i].dOctets;
        }
      }
    }
  }
}