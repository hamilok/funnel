#ifndef FUNNEL_STATISTIC_HPP
#define FUNNEL_STATISTIC_HPP

struct direction
{
    unsigned long incoming;
    unsigned long outgoing;

    direction() : incoming( 0 ), outgoing( 0 )
    {}
};

#endif /* FUNNEL_STATISTIC_HPP */
