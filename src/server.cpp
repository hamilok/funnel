#include "common.hpp"
#include "server.hpp"

server::server(short port)
	:	socket_(io_service_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
        timer_(io_service_, boost::posix_time::seconds(1)),
        packets_cnt_(0),
        packets_size_(0)
{
    abonent_list.push_back(boost::asio::ip::address_v4::from_string("101.122.123.77"));
    abonent_list.push_back(boost::asio::ip::address_v4::from_string("217.28.41.77"));
    abonent_list.push_back(boost::asio::ip::address_v4::from_string("233.236.73.93"));
    abonent_list.push_back(boost::asio::ip::address_v4::from_string("101.130.138.78"));
    abonent_list.push_back(boost::asio::ip::address_v4::from_string("5.195.144.82"));
    abonent_list.push_back(boost::asio::ip::address_v4::from_string("103.47.73.93"));

    std::sort(abonent_list.begin(), abonent_list.end());


    std::cout << "Loading the address list from www.colocall.net:" << std::endl;

    boost::asio::ip::tcp::iostream stream("www.colocall.net", "http");

    stream << "GET /ua/prefixes.txt HTTP/1.0\r\n";
    stream << "Host: www.colocall.net\r\n";
    stream << "Accept: */*\r\n";
    stream << "Connection: close\r\n\r\n";

    std::string prefix;

    // headers
    while (std::getline(stream, prefix) && prefix.size() != 1)
    {
        // TODO
    }

    // addresses
    while (std::getline(stream, prefix))
    {
        uaix_list.push_back(network_v4::from_string(prefix));
    }

    std::sort(uaix_list.begin(), uaix_list.end());

    std::cout << " - OK" << std::endl;

    // Start
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

    timer_.async_wait(boost::bind(&server::handle_timeout, this));
}

void server::run()
{
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (std::size_t i = 0; i < 4; ++i)
    {
        boost::shared_ptr<boost::thread> thread(new boost::thread(
            boost::bind(&boost::asio::io_service::run, &io_service_)));
        threads.push_back(thread);
    }

    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

void server::stop()
{
    io_service_.stop();
}

void server::handle_timeout()
{
    std::cout << "packets: " << packets_cnt_ << ", size: " << (packets_size_ / (1024 * 1024)) << " MB/s" << std::endl;

    packets_cnt_ = 0;
    packets_size_ = 0;

    timer_.expires_from_now(boost::posix_time::seconds(1));
    timer_.async_wait(boost::bind(&server::handle_timeout, this));
}

void server::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
    if (!error && bytes_recvd > 0)
    {
        packets_cnt_ += 1;
        packets_size_ += bytes_recvd;
/*
        nf_header hdr;
        nf_record rec;

        memcpy(&hdr, data_, sizeof(nf_header));

        unsigned int pos = sizeof(nf_header);
        for (unsigned short i = 0; i < ntohs(hdr.count); i++, pos += sizeof(nf_record))
        {
            memcpy(&rec, &data_[pos], sizeof(nf_record));

            boost::asio::ip::address_v4 src(rec.srcaddr);
            boost::asio::ip::address_v4 dst(rec.dstaddr);

            std::vector<boost::asio::ip::address_v4>::iterator abonent_itr = abonent_list.end();
            std::vector<network_v4>::iterator network_itr = uaix_list.end();

            abonent_itr = binary_search2(abonent_list.begin(), abonent_list.end(), src);

            // Abonent
            if ( abonent_itr != abonent_list.end() )
            {
                network_itr = binary_search2(uaix_list.begin(), uaix_list.end(), dst);

                // UA-IX
                if ( network_itr != uaix_list.end() )
                {
//                  std::cout << "form: ua-ix, to: " << *abonent_itr << std::endl;
                }
                else
                {
//                  std::cout << "form: world, to: " << *abonent_itr << std::endl;
                }
            }
            else
            {
                abonent_itr = binary_search2(abonent_list.begin(), abonent_list.end(), dst);

                // Abonent
                if ( abonent_itr != abonent_list.end() )
                {
                    network_itr = binary_search2(uaix_list.begin(), uaix_list.end(), src);

                    // UA-IX
                    if ( network_itr != uaix_list.end() )
                    {
//                      std::cout << "from ua-ix, to: " << *abonent_itr << std::endl;
                    }
                    else
                    {
//                      std::cout << "from world, to: " << *abonent_itr << std::endl;
                    }
                }
            }
        }
*/
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
