#ifndef FUNNEL_SERVER_HPP
#define FUNNEL_SERVER_HPP

#include <deque>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "netflow_header.hpp"
#include "netflow_record.hpp"

#include "network_v4.hpp"
#include "statistic.hpp"

class server : private boost::noncopyable
{
public:
	explicit server(std::size_t port, std::size_t thread_cnt, std::size_t buffer_size, std::size_t update_int);

    void run();
    void stop();

private:
    void handle_update_uaix();
    void handle_update_stat();
    void handle_receive_from(const boost::system::error_code& error, std::size_t bytes_recvd);

private:
	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket socket;
	boost::asio::ip::udp::endpoint sender_endpoint;
    boost::asio::deadline_timer timer_uaix;
    boost::asio::deadline_timer timer_stat;
    boost::mutex mutex;
    boost::mutex mutex2;

    std::vector< network_v4 > uaix_list;
    std::vector< unsigned long > abonent_list;
    statistic_list_t statistic_list;

    std::deque< nf_record > records_queue;


    enum
    {
        max_length = 65535
    };

    char data[max_length];

    std::size_t header_size;
    std::size_t record_size;

    std::size_t thread_cnt;
    std::size_t update_int;

    std::size_t flows_cnt;
    std::size_t bytes_cnt;
};

#endif /* FUNNEL_SERVER_HPP */
