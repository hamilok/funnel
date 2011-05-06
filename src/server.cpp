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
     buff_pool(thread_cnt)
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
    boost::shared_ptr< boost::thread > thread(
      new boost::thread(
        boost::bind(
          &boost::asio::io_service::run,
          &io_service
        )
      )
    );
    threads.push_back(thread);
  }
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
  
  std::cout << std::endl;
}

void server::list_zones()
{
  zone_mgr.print();
}

void server::clear_zones()
{
  zone_mgr.clear();
  std::cout << "Zones cleared" << std::endl;
}

void server::load_abonents(const std::string& filename)
{
  std::cout << "Loading abonents from \"" << filename << "\": ";
  
  if (abonent_mgr.load(filename))
    std::cout << "OK";
  else
    std::cout << "Failed";
  
  std::cout << std::endl;
  
}

void server::list_abonents()
{
  abonent_mgr.print();
}

void server::clear_abonents()
{
  abonent_mgr.clear();
  std::cout << "Abonents cleared" << std::endl;
}

void server::statistic_dump()
{
/*
  std::ofstream file("statistic_dump.bin", std::ios::out | std::ios::binary);
  if (file.is_open())
  {
    for (std::size_t i = 0; i < client_list.size(); i++)
      file << client_list[i];
  }
  file.close();
*/
}

void server::handle_update_abonents()
{
  abonent_mgr.update();

  abonent_timer.expires_from_now ( boost::posix_time::seconds ( update_int ) );
  abonent_timer.async_wait ( boost::bind ( &server::handle_update_abonents, this ) );
}

void server::handle_update_zones()
{
  zone_mgr.update();

  zone_timer.expires_from_now( boost::posix_time::seconds( update_int ) );
  zone_timer.async_wait( boost::bind( &server::handle_update_zones, this ) );
}

void server::start_receive()
{
  socket.async_receive_from (
    boost::asio::buffer ( data, max_length ),
    sender_endpoint,
    boost::bind (
      &server::handle_receive,
      this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}

void server::handle_receive ( const boost::system::error_code& error, std::size_t bytes_recvd )
{
  // Is error
  if ( error || bytes_recvd == 0 )
  {
    std::cout << "Unknown error" << std::endl;
    return start_receive();
  }

  // Data is not netflow version 5
  if ( packet.hdr.get_version() != 5 )
  {
    std::cout << "Error: received unknwon packet version" << std::endl;    
    return start_receive();
  }
    
  std::cout << boost::this_thread::get_id() << ": process" << std::endl;

  unsigned char zone_code = 0;
  std::vector< zone >::iterator network_itr;
  std::vector< abonent >::iterator client_itr;
/*
  // Each per flow
  for ( std::size_t i = 0; i < packet.hdr.get_count (); i++ )
  {
    // reset code
    zone_code = 0;

    client_itr = binary_search2 ( client_list.begin (), client_list.end (), packet.recs[ i ].srcaddr );

    if ( client_itr != client_list.end () )
    {
      network_itr = binary_search2 ( network_list.begin (), network_list.end (), pkt.recs[ i ].dstaddr );

      if ( network_itr != network_list.end () )
      {
        zone_code = ( *network_itr ).get_code();
      }

      ( *client_itr ).dir[ zone_code ].outgoing += packet.recs[ i ].dOctets;
    }
    else
    {
      client_itr = binary_search2 ( client_list.begin (), client_list.end (), packet.recs[ i ].dstaddr );

      if ( client_itr != client_list.end () )
      {
        network_itr = binary_search2 ( network_list.begin (), network_list.end (), pkt.recs[ i ].srcaddr );

        if ( network_itr != network_list.end () )
        {
          zone_code = ( *network_itr ).get_code ();
        }

        ( *client_itr ).dir[ zone_code ].incoming += packet.recs[ i ].dOctets;
      }
    }
  }
*/
  start_receive();
}
