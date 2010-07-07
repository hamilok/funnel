#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/asio.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "boost/asio/ip/network_v4.hpp"

#include "common.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Loading the address list from www.colocall.net:" << std::endl;

    boost::asio::ip::tcp::iostream stream("www.colocall.net", "http");

    stream << "GET /ua/prefixes.txt HTTP/1.0\r\n";
    stream << "Host: www.colocall.net\r\n";
    stream << "Accept: */*\r\n";
    stream << "Connection: close\r\n\r\n";

    std::string prefix;
    std::vector<boost::asio::ip::network_v4> network_list;

    // headers
    while (std::getline(stream, prefix) && prefix.size() != 1)
    {
        // TODO
    }

    // addresses
    while (std::getline(stream, prefix))
    {
        network_list.push_back(boost::asio::ip::network_v4::from_string(prefix));
    }

    std::cout << " - Found " << network_list.size() << " unique prefixes" << std::endl;


    std::cout << "Sorting:" << std::endl;


    boost::posix_time::ptime tStart = boost::posix_time::microsec_clock::local_time();

    std::sort(network_list.begin(), network_list.end());

    boost::posix_time::ptime tEnd = boost::posix_time::microsec_clock::local_time();

    boost::posix_time::time_duration tVal = tEnd - tStart;

    std::cout << " - OK " << boost::posix_time::to_simple_string(tVal) << std::endl;




    boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string("62.72.190.25");


    tStart = boost::posix_time::microsec_clock::local_time();

    if (binary_search(network_list.begin(), network_list.end(), addr))
    {
        tEnd = boost::posix_time::microsec_clock::local_time();
        tVal = tEnd - tStart;

        std::cout << addr << " is ua-ix " << boost::posix_time::to_simple_string(tVal) << std::endl;
    }


    return 0;
}
