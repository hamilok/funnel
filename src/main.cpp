#include <cstdlib>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <signal.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <boost/lexical_cast.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include "server.hpp"

class subscriber {
public:
	subscriber(boost::asio::ip::address_v4& addr) : address(addr), incoming(0), outgoing(0), direction(0) {}

private:
	boost::asio::ip::address_v4 address;
	unsigned long incoming;
	unsigned long outgoing;
	unsigned char direction;
};

template <class OutputIterator>
OutputIterator expand_network(OutputIterator result, const std::string& network)
{
	std::size_t found = network.find_last_of("/");
	boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(network.substr(0, found));

	unsigned short bits = boost::lexical_cast<unsigned short>(network.substr(found + 1));

	unsigned long mask = 0;
	for (int i = 0; i < bits; i++)
	{
		mask |= 1 << (31 - i);
	}

	unsigned long net = addr.to_ulong() & mask;

	for (unsigned long i = (~mask & 1); i <= (~mask & 0xfffffffe); i++)
	{
		*result++ = boost::asio::ip::address_v4(net | i);
	}

	return result;
}

boost::asio::io_service io_service;
boost::asio::deadline_timer timer(io_service);
std::vector<boost::asio::ip::address> address_list;

void get_uaix(std::vector<boost::asio::ip::address>& address_list)
{
	std::cout << "Loading the address list: ";
/*
	boost::asio::ip::tcp::iostream stream("www.colocall.net", "http");

	stream << "GET /ua/prefixes.txt HTTP/1.0\r\n";
	stream << "Host: www.colocall.net\r\n";
	stream << "Accept: *\/*\r\n";
	stream << "Connection: close\r\n\r\n";

    std::string address;

    // headers
    while (std::getline(stream, address) && address.size() != 1)
    {
    	// TODO
    }

    // addresses
    while (std::getline(stream, address))
    {
    	expand_network(back_inserter(address_list), address);
    }

	std::sort(address_list.begin(), address_list.end());
*/
    timer.expires_from_now(boost::posix_time::seconds(10));
    timer.async_wait(boost::bind(get_uaix, address_list));
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
			return 1;
		}

		get_uaix(address_list);

		std::cout << address_list.size() << std::endl;

		server srv(io_service, address_list, std::atoi(argv[1]));

		io_service.run();
/*
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
				std::cout << "found " << address_list.size() << " unique address:" << std::endl;
			}
			else if (!line.empty())
			{
				std::cout << "Unknown command: " << line << std::endl;
			}
			std::cout << "server> ";
		}
		while (std::cin >> line);

		t.join();
*/

		std::cout << "found " << address_list.size() << " unique address:" << std::endl;
		for (std::size_t i = 0; i < address_list.size(); i++)
		{
			std::cout << " - " << address_list[i] << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
