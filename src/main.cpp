#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

#include "config.hpp"
#include "server.hpp"

int main( int argc, char** argv )
{
    try
    {
        std::string address = "0.0.0.0";
        std::size_t port = 9500;
        std::size_t thread_cnt = boost::thread::hardware_concurrency();
        std::size_t buffer_size = 65535;
        std::size_t update_int = 900;

        boost::program_options::options_description generic( "Generic options" );
        generic.add_options()
            ( "version,v", "Print version string" )
            ( "help,h",    "Produce help message" );

        boost::program_options::options_description config( "Configuration" );
        config.add_options()
            ( "address,a",   boost::program_options::value< std::string >( &address )->default_value( address ),         "IP address. By default, bind to any address." )
            ( "port,p",      boost::program_options::value< std::size_t >( &port )->default_value( port ),               "Port number." )
            ( "threads,t",   boost::program_options::value< std::size_t >( &thread_cnt )->default_value( thread_cnt ),   "Threads count. By default, the maximum available." )
            ( "buffer,b",    boost::program_options::value< std::size_t >( &buffer_size )->default_value( buffer_size ), "Socket receiver buffer size." )
            ( "update,u",    boost::program_options::value< std::size_t >( &update_int )->default_value( update_int ),   "Update interval in seconds." );

        boost::program_options::options_description hidden( "Hidden options" );
        hidden.add_options()
            ( "client-list", boost::program_options::value< std::vector< std::string > >(), "client list" )
            ( "network-list", boost::program_options::value< std::vector< std::string > >(), "network list" );

        boost::program_options::options_description cmdline_options;
        cmdline_options.add( generic ).add( config ).add( hidden );

        boost::program_options::options_description config_file_options;
        config_file_options.add( config ).add( hidden );

        boost::program_options::options_description visible;
        visible.add( generic ).add( config );

        boost::program_options::variables_map vm;
        boost::program_options::store( boost::program_options::command_line_parser( argc, argv ).options( cmdline_options ).run(), vm );
        boost::program_options::notify( vm );

        if ( vm.count( "help" ) )
        {
            std::cout << cmdline_options << std::endl;
            return 1;
        }
        else if ( vm.count( "version" ) )
        {
            std::cout << "funnel version " << FUNNEL_VERSION_MAJOR << "." << FUNNEL_VERSION_MINOR << std::endl;
            return 1;
        }

        /**
         * Start server
         * 
         * 
         */
        server srv (
            address,
            port,
            thread_cnt,
            buffer_size,
            update_int
        );
        srv.run ();
        
        std::string cmd;
        boost::smatch results;
        while ( true )
        {
            std::cout << std::endl;
            std::cout << "funnel> ";
            std::getline ( std::cin, cmd );

            if ( boost::regex_match ( cmd, results, boost::regex ( "(stop|exit|quit)" ) ) )
            {
                srv.stop();
                break;
            }
            else if ( cmd == "help" )
            {
                std::cout << "client ( list | load | clear )" << std::endl;
                std::cout << "network ( list | clear )" << std::endl;
            }
            else if ( boost::regex_match ( cmd, results, boost::regex ( "client list" ) ) )
            {
                srv.list_abonent();
            }
            else if ( boost::regex_match ( cmd, results, boost::regex ( "client clear" ) ) )
            {
                srv.clear_abonent();
            }
            else if ( boost::regex_match ( cmd, results, boost::regex ( "network list" ) ) )
            {
                srv.list_zone();
            }
            else if ( boost::regex_match ( cmd, results, boost::regex ( "network clear" ) ) )
            {
                srv.clear_zone();
            }
            else if ( boost::regex_match ( cmd, results, boost::regex ( "statistic dump" ) ) )
            {
                srv.statistic_dump();
            }
            else if ( cmd.length() != 0 )
            {
                std::cout << "Unknown command" << std::endl;
            }
        }

        srv.wait ();
    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
