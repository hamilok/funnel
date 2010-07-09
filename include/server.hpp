#ifndef FUNNEL_SERVER_HPP
#define FUNNEL_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "netflow_header.hpp"
#include "netflow_record.hpp"

#include "network_v4.hpp"

class server : private boost::noncopyable
{
public:
	explicit server(short port);

    void run();
    void stop();

private:
    void handle_timeout();
    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);

private:
	boost::asio::io_service io_service_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
    std::vector<boost::asio::ip::address_v4> abonent_list;
    std::vector<network_v4> uaix_list;
	enum
	{
		max_length = 65535
	};
	char data_[max_length];
    boost::asio::deadline_timer timer_;
    unsigned long packets_cnt_;
    unsigned long packets_size_;
};

#endif /* FUNNEL_SERVER_HPP */
