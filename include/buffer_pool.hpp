#ifndef FUNNEL_BUFFER_POOL_HPP
#define FUNNEL_BUFFER_POOL_HPP

#include <boost/noncopyable.hpp>

template < class T >
class buffer_pool
{
public:
    explicit buffer_pool ( std::size_t pool_size ) : next ( 0 )
    {
        buffers.reserve ( pool_size );
    }

    T& get_buffer()
    {
        T buf = buffers [ next++ ];
        if ( next == buffers.size () )
            next = 0;
        return buf;
    }

private:
    std::size_t next;
    std::vector < T > buffers;
};

#endif /* FUNNEL_BUFFER_POOL_HPP */
