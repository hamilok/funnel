#include "common.hpp"
#include "server.hpp"

server::server ( const std::string& address, std::size_t port, std::size_t thread_cnt, std::size_t buffer_size, std::size_t update_int )
    :   socket ( io_service, boost::asio::ip::udp::endpoint ( boost::asio::ip::address::from_string ( address ), port ) ),
        client_timer ( io_service, boost::posix_time::seconds ( update_int ) ),
        network_timer ( io_service, boost::posix_time::seconds ( update_int ) ),
        thread_cnt ( thread_cnt ),
        update_int ( update_int ),
        buffer_size ( buffer_size ),
        flows_cnt ( 0 ),
        bytes_cnt ( 0 ),
        running ( true ),
        client_file ( "./clients.dat" ),
        network_file ( "./networks.dat" ),
        buff_pool ( thread_cnt )
{
    /// Set receive buffer size for socket
    boost::asio::socket_base::receive_buffer_size option ( buffer_size );
    socket.set_option ( option );
    socket.get_option ( option );
    std::cout << "Socket receive buffer size: " <<  option.value () << std::endl;

    // Start
    socket.async_receive_from (
        boost::asio::buffer ( data, max_length ),
        sender_endpoint,
        boost::bind (
            &server::handle_receive_from,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );

    client_timer.async_wait( boost::bind( &server::handle_update_client, this ) );
    network_timer.async_wait( boost::bind( &server::handle_update_network, this ) );
}

void server::run ()
{
    for ( std::size_t i = 0; i < thread_cnt; ++i )
    {
        boost::shared_ptr< boost::thread > thread(
            new boost::thread(
                boost::bind(
                    &boost::asio::io_service::run,
                    &io_service
                )
            )
        );
        threads.push_back( thread );
    }
}

void server::wait ()
{
    for ( std::size_t i = 0; i < threads.size(); ++i )
        threads[i]->join();
}

void server::stop ()
{
    running = false;
    io_service.stop();
}

void server::list_zone ()
{
    boost::mutex::scoped_lock lock( mutex );

    for ( std::size_t i = 0; i < network_list.size(); i++ )
        std::cout << network_list[ i ] << std::endl;
    std::cout << "total count: " << network_list.size() << std::endl;
}

void server::clear_zone ()
{
    boost::mutex::scoped_lock lock( mutex );

    network_list.clear();
}

void server::list_abonent ()
{
    boost::mutex::scoped_lock lock( mutex );

    for ( std::size_t i = 0; i < client_list.size(); i++ )
        std::cout << client_list[i] << std::endl;
    std::cout << "total count: " << client_list.size() << std::endl;
}

void server::clear_abonent ()
{
    boost::mutex::scoped_lock lock( mutex );

    client_list.clear();
}

void server::statistic_dump ()
{
    boost::mutex::scoped_lock lock( mutex );

    std::ofstream file ( "statistic_dump.bin", std::ios::out | std::ios::binary );
    if ( file.is_open() )
    {
        for ( std::size_t i = 0; i < client_list.size(); i++ )
            file << client_list[i];
    }
    file.close();
}

void server::handle_update_client ()
{
    try
    {
        std::size_t filesize = boost::filesystem::file_size ( client_file );
        std::time_t current_timestamp = boost::filesystem::last_write_time ( client_file );

        // Calc checksum only modified and not empty file
        if ( current_timestamp > client_timestamp && filesize > 0 )
        {
            client_timestamp = current_timestamp;

            checksum_md5 md5;
            if ( !md5.compare_file ( client_file, client_checksum ) )
            {
                boost::filesystem::ifstream file ( client_file, std::ios::binary );

                if ( file.is_open () )
                {
                    boost::mutex::scoped_lock lock( mutex );

                    client_list.clear ();

                    std::string record;
                    boost::smatch result;

                    while ( !std::getline ( file, record ) .eof () )
                    {
                        if ( !boost::regex_match ( record, result, boost::regex ( "([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})" ) ) )
                            continue;

                        client_list.push_back (
                            abonent (
                                boost::asio::ip::address_v4::from_string ( result [ 1 ] ).to_ulong (),
                                "EE:EE:EE:EE:EE:EE"
                            )
                        );
                    }
                    std::sort ( client_list.begin (), client_list.end () );
                }
                file.close ();
            }
        }
    }
    catch ( boost::filesystem::filesystem_error& e )
    {
        std::cerr << e.what () << std::endl;
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what () << std::endl;
    }

    client_timer.expires_from_now ( boost::posix_time::seconds ( update_int ) );
    client_timer.async_wait ( boost::bind ( &server::handle_update_client, this ) );
}

void server::handle_update_network()
{
    try
    {
        std::size_t filesize = boost::filesystem::file_size ( network_file );
        std::time_t current_timestamp = boost::filesystem::last_write_time ( network_file );

        // Calc checksum only modified and not empty file
        if ( current_timestamp > network_timestamp && filesize > 0 )
        {
            network_timestamp = current_timestamp;

            checksum_md5 md5;
            if ( !md5.compare_file ( network_file, network_checksum ) )
            {
                boost::filesystem::ifstream file ( network_file, std::ios::binary );

                if ( file.is_open () )
                {
                    boost::mutex::scoped_lock lock( mutex );

                    network_list.clear ();

                    std::string record;
                    boost::smatch result;

                    while ( !std::getline ( file, record ) .eof () )
                    {
                        if ( !boost::regex_match ( record, result, boost::regex ( "([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})/([0-9]{2})" ) ) )
                            continue;

                        network_list.push_back (
                            zone (
                                boost::asio::ip::address_v4::from_string( result [ 1 ] ).to_ulong (),
                                atoi ( result [ 2 ].str().c_str() ),
                                1
                            )
                        );

                    }
                    std::sort ( network_list.begin (), network_list.end () );
                }
                file.close ();
            }
        }
    }
    catch ( boost::filesystem::filesystem_error& e )
    {
        std::cerr << e.what () << std::endl;
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what () << std::endl;
    }

    network_timer.expires_from_now( boost::posix_time::seconds( update_int ) );
    network_timer.async_wait( boost::bind( &server::handle_update_network, this ) );
}

void server::handle_receive_from( const boost::system::error_code& error, std::size_t bytes_recvd )
{
/*
    nf_packet pkt;
    {
        boost::mutex::scoped_lock lock ( mutex2 );
        memcpy ( &pkt, &data, sizeof ( nf_packet ) );
    }

    socket.async_receive_from (
        boost::asio::buffer ( data, max_length ),
        sender_endpoint,
        boost::bind (
            &server::handle_receive_from,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
*/

    // New data
    if ( !error && bytes_recvd > 0 )
    {
        // Data is netflow version 5
        if ( packet.hdr.get_version() == 5 )
        {
            // For client & network
            boost::mutex::scoped_lock lock ( mutex );

            std::cout << boost::this_thread::get_id() << ": process" << std::endl;

            unsigned char zone_code = 0;
            std::vector< zone >::iterator network_itr;
            std::vector< abonent >::iterator client_itr;

            // Each per flow
            for ( std::size_t i = 0; i < packet.hdr.get_count (); i++ )
            {
                // reset code
                zone_code = 0;

                client_itr = binary_search2 ( client_list.begin (), client_list.end (), packet.recs[ i ].srcaddr );

                if ( client_itr != client_list.end () )
                {
//                    network_itr = binary_search2 ( network_list.begin (), network_list.end (), pkt.recs[ i ].dstaddr );

//                    if ( network_itr != network_list.end () )
//                    {
//                        zone_code = ( *network_itr ).get_code();
//                    }

                    ( *client_itr ).dir[ zone_code ].outgoing += packet.recs[ i ].dOctets;
                }
                else
                {
                    client_itr = binary_search2 ( client_list.begin (), client_list.end (), packet.recs[ i ].dstaddr );

                    if ( client_itr != client_list.end () )
                    {
//                        network_itr = binary_search2 ( network_list.begin (), network_list.end (), pkt.recs[ i ].srcaddr );

//                        if ( network_itr != network_list.end () )
//                        {
//                            zone_code = ( *network_itr ).get_code ();
//                        }

                        ( *client_itr ).dir[ zone_code ].incoming += packet.recs[ i ].dOctets;
                    }
                }
            }
        }
        else
            std::cout << "Error: received unknwon packet version" << std::endl;
    }

    socket.async_receive_from (
        boost::asio::buffer ( data, max_length ),
        sender_endpoint,
        boost::bind (
            &server::handle_receive_from,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );

}
