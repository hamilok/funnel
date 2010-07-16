#include "common.hpp"
#include "server.hpp"

server::server( std::size_t port, std::size_t thread_cnt, std::size_t buffer_size, std::size_t update_int )
    :   socket( io_service, boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4(), port ) ),
        timer( io_service, boost::posix_time::seconds( 0 ) ),
        thread_cnt( thread_cnt ),
        update_int( update_int ),
        flows_cnt( 0 )
{
    /// Set receive buffer size for socket
    boost::asio::socket_base::receive_buffer_size option( buffer_size );
    socket.set_option( option );
    socket.get_option( option );
    std::cout << "Socket receive buffer size: " <<  option.value() << std::endl;

    /// Fill
    abonent_list.push_back( boost::asio::ip::address_v4::from_string( "101.122.123.77" ).to_ulong() );
    abonent_list.push_back( boost::asio::ip::address_v4::from_string( "217.28.41.77"   ).to_ulong() );
    abonent_list.push_back( boost::asio::ip::address_v4::from_string( "233.236.73.93"  ).to_ulong() );
    abonent_list.push_back( boost::asio::ip::address_v4::from_string( "101.130.138.78" ).to_ulong() );
    abonent_list.push_back( boost::asio::ip::address_v4::from_string( "5.195.144.82"   ).to_ulong() );
    abonent_list.push_back( boost::asio::ip::address_v4::from_string( "103.47.73.93"   ).to_ulong() );

    /// Sort
    std::sort( abonent_list.begin(), abonent_list.end() );

    /// Perset
    header_size = sizeof( nf_header );
    record_size = sizeof( nf_record );

    // Start
    socket.async_receive_from(
        boost::asio::buffer( data, max_length ),
        sender_endpoint,
        boost::bind(
            &server::handle_receive_from,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );

    timer.async_wait( boost::bind( &server::handle_update, this ) );
}

void server::run()
{
    std::vector <boost::shared_ptr< boost::thread > > threads;
    for ( std::size_t i = 0; i < thread_cnt; ++i )
    {
        boost::shared_ptr <boost::thread > thread(
            new boost::thread(
                boost::bind(
                    &boost::asio::io_service::run,
                    &io_service
                )
            )
        );
        threads.push_back( thread );
    }

    for ( std::size_t i = 0; i < threads.size(); ++i )
        threads[i]->join();
}

void server::stop()
{
    io_service.stop();
}

void server::handle_update()
{
    boost::mutex::scoped_lock lock( mutex );

    std::cout << "Loading the address list from www.colocall.net: ";
    std::cout.flush();

    boost::asio::ip::tcp::iostream stream( "www.colocall.net", "http" );

    stream << "GET /ua/prefixes.txt HTTP/1.0\r\n";
    stream << "Host: www.colocall.net\r\n";
    stream << "Accept: */*\r\n";
    stream << "Connection: close\r\n\r\n";

    std::string prefix;

    // headers
    while ( std::getline( stream, prefix ) && prefix.size() != 1 )
    {
        // TODO
    }

    std::size_t prev_size = uaix_list.size();

    uaix_list.clear();

    // addresses
    while ( std::getline( stream, prefix ) )
    {
        uaix_list.push_back( network_v4::from_string( prefix ) );
    }

    std::sort( uaix_list.begin(), uaix_list.end() );

    std::size_t curr_size = uaix_list.size();

    std::cout << "OK (" << prev_size << "->" << curr_size << ")" << std::endl;

    for ( std::size_t i = 0; i < statistic_list.size(); i++ )
    {
        std::cout << statistic_list[ i ] << std::endl;
    }

    timer.expires_from_now( boost::posix_time::seconds( update_int ) );
    timer.async_wait( boost::bind( &server::handle_update, this ) );
}

void server::handle_receive_from( const boost::system::error_code& error, std::size_t bytes_recvd )
{
    if ( !error && bytes_recvd > 0 )
    {
        unsigned char direct = 0;

        nf_header hdr;
        memcpy( &hdr, data, header_size );

        if ( hdr.get_version() == 5 )
        {
            //flows_cnt += hdr.get_count();

            //std::cout << "Flows: " << flows_cnt << "\r";
            //std::cout.flush();

            nf_record* recs = new nf_record[ hdr.get_count() ];
            memcpy( recs, &data[ header_size ], hdr.get_count() * record_size );


            std::vector< network_v4 >::iterator network_itr = uaix_list.end();
            std::vector< unsigned long >::iterator abonent_itr = abonent_list.end();
            statistic_list_i statistic_itr = statistic_list.end();

            for ( unsigned short i = 0; i < hdr.get_count(); i++ )
            {
                abonent_itr = binary_search2( abonent_list.begin(), abonent_list.end(), recs[ i ].srcaddr );

                // Abonent
                if ( abonent_itr != abonent_list.end() )
                {
                    statistic_itr = lower_bound( statistic_list.begin(), statistic_list.end(), recs[ i ].srcaddr );

                    if ( statistic_itr == statistic_list.end() )
                    {
                        statistic_list.insert( statistic_list.end(), statistic_t( recs[ i ].srcaddr ) );
                        statistic_itr = statistic_list.begin() + statistic_list.size() - 1;
                    }
                    else if (statistic_itr < statistic_list.begin() )
                    {
                        statistic_list.insert( statistic_itr, statistic_t( recs[ i ].srcaddr ) );
                    }


                    network_itr = binary_search2( uaix_list.begin(), uaix_list.end(), recs[ i ].dstaddr );

                    // UA-IX
                    if ( network_itr != uaix_list.end() )
                    {
                        direct = 1;
//                      std::cout << "form: ua-ix, to: " << *abonent_itr << std::endl;
                    }
                    else
                    {
                        direct = 2;
//                      std::cout << "form: world, to: " << *abonent_itr << std::endl;
                    }

                    ( *statistic_itr ).direction[ direct ].incoming += recs[ i ].dOctets;
                }
                else
                {
                    abonent_itr = binary_search2( abonent_list.begin(), abonent_list.end(), recs[ i ].dstaddr );

                    // Abonent
                    if ( abonent_itr != abonent_list.end() )
                    {
                        statistic_itr = lower_bound( statistic_list.begin(), statistic_list.end(), recs[ i ].srcaddr );

                        if ( statistic_itr == statistic_list.end() )
                        {
                            statistic_list.insert( statistic_list.end(), statistic_t( recs[ i ].srcaddr ) );
                            statistic_itr = statistic_list.begin() + statistic_list.size() - 1;
                        }
                        else if (statistic_itr < statistic_list.begin() )
                        {
                            statistic_list.insert( statistic_list.begin(), statistic_t( recs[ i ].srcaddr ) );
                        }

                        network_itr = binary_search2( uaix_list.begin(), uaix_list.end(), recs[ i ].srcaddr );

                        // UA-IX
                        if ( network_itr != uaix_list.end() )
                        {
                            direct = 1;
//                          std::cout << "from ua-ix, to: " << *abonent_itr << std::endl;
                        }
                        else
                        {
                            direct = 2;
//                          std::cout << "from world, to: " << *abonent_itr << std::endl;
                        }

                        ( *statistic_itr).direction[ direct ].outgoing += recs[ i ].dOctets;
                    }
                }
            }


            delete recs;
        }
        else
            std::cout << "Error: received unknwon packet version" << std::endl;

    }

    socket.async_receive_from(
        boost::asio::buffer( data, max_length ),
        sender_endpoint,
        boost::bind(
            &server::handle_receive_from,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}
