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

#include "zone.hpp"
#include "abonent.hpp"

#include <boost-1_42/boost/concept_check.hpp>

class server : private boost::noncopyable
{
public:
  explicit server ( const std::string& address, std::size_t port, std::size_t thread_cnt, std::size_t buffer_size, std::size_t update_int );

  void run ();
  void wait ();
  void stop ();

  void list_zone ();
  void clear_zone ();

  void list_abonent ();
  void clear_abonent ();

  void statistic_dump();

private:
  void handle_process ();

  void handle_update_client ();
  void handle_update_network ();
  void handle_update_stat ();

  void handle_receive_from ( const boost::system::error_code& error, std::size_t bytes_recvd );

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
  boost::filesystem::path network_file;
  std::time_t network_timestamp;
  unsigned char network_checksum[MD5_DIGEST_LENGTH];
  std::vector< zone > network_list;
  boost::asio::deadline_timer network_timer;

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
