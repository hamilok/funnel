#include "common.hpp"
#include "server.hpp"

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
			return 1;
		}

		boost::asio::io_service io_service;
		boost::asio::deadline_timer timer(io_service);

		std::vector<subscriber> network_list;
        std::vector<subscriber> abonent_list;

        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.234"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.236"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.238"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.240"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.242"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.244"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.246"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.248"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.250"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.252"), 0, 0, 0));
        abonent_list.push_back(subscriber(boost::asio::ip::address_v4::from_string("217.199.237.254"), 0, 0, 0));

		get_uaix(network_list);

        std::cout << "Listening..." << std::endl;

		server srv(io_service, std::atoi(argv[1]), network_list, abonent_list);

		io_service.run();

		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

		std::string line;
		do
		{
			if (line == "stop")
			{
				io_service.stop();
				break;
			}
			else if (line == "print")
			{
                for (std::size_t i = 0; i < abonent_list.size(); i++)
                {
                    std::cout << abonent_list[i].get_address() << ": " << abonent_list[i].get_incoming() << "/" << abonent_list[i].get_outgoing() << std::endl;
                }
			}
			else if (!line.empty())
			{
				std::cout << "Unknown command: " << line << std::endl;
			}
			std::cout << "server> ";
		}
		while (std::cin >> line);

		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
