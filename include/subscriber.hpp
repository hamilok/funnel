#ifndef FUNNEL_SUBSCRIBER_HPP
#define FUNNEL_SUBSCRIBER_HPP

struct subscriber {
    boost::asio::ip::address_v4 address;
    unsigned long incoming;
    unsigned long outgoing;
    unsigned char direction;

    subscriber(boost::asio::ip::address_v4 addr, unsigned long in, unsigned long out, unsigned char dir) : address(addr), incoming(in), outgoing(out), direction(dir) {}
};

#endif /* FUNNEL_SUBSCRIBER_HPP */
