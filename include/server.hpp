/*
 * server.hpp
 *
 *  Created on: Jun 17, 2010
 *      Author: hamilok
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "netflow_header.hpp"
#include "netflow_record.hpp"

class server
{
public:
	server(boost::asio::io_service& io_service, std::vector<boost::asio::ip::address>& alist, short port) :
		io_service_(io_service),
		addr_list(alist),
		socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	{
		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length),
			sender_endpoint_,
			boost::bind(
				&server::handle_receive_from,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
	{
		if (!error && bytes_recvd > 0)
		{
			nf_header hdr;
			nf_record rec;

			memcpy(&hdr, data_, sizeof(nf_header));

			unsigned int pos = sizeof(nf_header);
			for (unsigned short i = 0; i < ntohs(hdr.count); i++, pos += sizeof(nf_record))
			{
				memcpy(&rec, &data_[pos], sizeof(nf_record));

				boost::asio::ip::address addr = boost::asio::ip::address_v4(rec.srcaddr);

				std::vector<boost::asio::ip::address>::iterator itr = lower_bound(addr_list.begin(), addr_list.end(), addr);

				if (itr == addr_list.end())
				{
					addr_list.push_back(addr);
				}
			}
		}

		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length),
			sender_endpoint_,
			boost::bind(
				&server::handle_receive_from,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

private:
	std::vector<boost::asio::ip::address>& addr_list;
	boost::asio::io_service& io_service_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	enum
	{
		max_length = 65535
	};
	char data_[max_length];
};

#endif /* SERVER_HPP_ */
