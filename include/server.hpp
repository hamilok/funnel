#ifndef FUNNEL_SERVER_HPP
#define FUNNEL_SERVER_HPP

class server
{
public:
	server(boost::asio::io_service& io_service, short port, std::vector<subscriber> networks, std::vector<subscriber> abonents) :
		io_service_(io_service),
		socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port),
        network_list(networks),
        abonent_list(abonents))
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

				subscriber src = subscriber(boost::asio::ip::address_v4(rec.srcaddr), 0, 0, 0);
                subscriber dst = subscriber(boost::asio::ip::address_v4(rec.dstaddr), 0, 0, 0);

				std::vector<subscriber>::iterator abonent_itr = abonent_list.end();
                std::vector<subscriber>::iterator network_itr = network_list.end();

                abonent_itr = lower_bound(abonent_list.begin(), abonent_list.end(), src, subscriber_sort_by_address);
				if (abonent_itr != abonent_list.end())
				{
                    (*abonent_itr).add_outgoing(rec.dOctets);

                    network_itr = lower_bound(network_list.begin(), network_list.end(), dst, subscriber_sort_by_address);
                    if (network_itr != network_list.end())
                    {
                        (*abonent_itr).set_direction((*network_itr).get_direction());
                    }
				}

                abonent_itr = lower_bound(abonent_list.begin(), abonent_list.end(), dst, subscriber_sort_by_address);
                if (abonent_itr != abonent_list.end()) 
                {
                    (*abonent_itr).add_incoming(rec.dOctets);

                    network_itr = lower_bound(network_list.begin(), network_list.end(), src, subscriber_sort_by_address);
                    if (network_itr != network_list.end()) 
                    {
                        (*abonent_itr).set_direction((*network_itr).get_direction());
                    }
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
	boost::asio::io_service& io_service_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
    std::vector<subscriber> network_list;
    std::vector<subscriber> abonent_list;
	enum
	{
		max_length = 65535
	};
	char data_[max_length];
};

#endif /* FUNNEL_SERVER_HPP */
