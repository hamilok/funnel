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

        std::cout << "Listening..." << std::endl;

		server srv(io_service, std::atoi(argv[1]));

        io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
