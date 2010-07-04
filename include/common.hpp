#ifndef FUNNEL_COMMON_HPP
#define FUNNEL_COMMON_HPP

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

#include "subscriber.hpp"
#include "netflow_header.hpp"
#include "netflow_record.hpp"

bool subscriber_sort_by_address(subscriber a, subscriber b)
{
    return (a.get_address() < b.get_address());
}

template <class OutputIterator>
OutputIterator expand_network(OutputIterator result, const std::string& network, unsigned short direction = 0)
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
        *result++ = subscriber(boost::asio::ip::address_v4(net | i), 0, 0, direction);
    }

    return result;
}

void get_uaix(std::vector<subscriber>& network_list)
{
    std::cout << "Loading the address list from www.colocall.net:" << std::endl;

    boost::asio::ip::tcp::iostream stream("www.colocall.net", "http");

    stream << "GET /ua/prefixes.txt HTTP/1.0\r\n";
    stream << "Host: www.colocall.net\r\n";
    stream << "Accept: */*\r\n";
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
        expand_network(back_inserter(network_list), address, 1);
    }

    std::cout << " - Found " << network_list.size() << " unique addresses" << std::endl;


    std::cout << "Sorting:" << std::endl;

    std::sort(network_list.begin(), network_list.end(), subscriber_sort_by_address);

    std::cout << " - OK" << std::endl;
}

#endif /* FUNNEL_COMMON_HPP */
