#ifndef FUNNEL_SUBSCRIBER_HPP
#define FUNNEL_SUBSCRIBER_HPP

class subscriber {
public:
    subscriber(boost::asio::ip::address_v4 addr, unsigned long in, unsigned long out, unsigned short dir) : address(addr), incoming(in), outgoing(out), direction(dir) {}

    boost::asio::ip::address get_address() { return address; }

    void add_incoming(unsigned long in) { incoming += in; }
    unsigned long get_incoming() { return incoming; }
    void add_outgoing(unsigned long out) { outgoing += out; }
    unsigned long get_outgoing() { return outgoing; }

    unsigned short get_direction() { return direction; }
    void set_direction(unsigned short dir) { direction = dir; }

private:
    boost::asio::ip::address_v4 address;
    unsigned long incoming;
    unsigned long outgoing;
    unsigned short direction;
};

#endif /* FUNNEL_SUBSCRIBER_HPP */
