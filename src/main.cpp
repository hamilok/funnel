#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

#include "server.hpp"

int main( int argc, char** argv )
{
    try
    {
        std::string interface;
        std::size_t port = 9500;
        std::size_t thread_cnt = 2;
        std::size_t buffer_size = 65535;
        std::size_t update_int = 900;

        boost::program_options::options_description generic( "Generic options" );
        generic.add_options()
            ( "version,v", "Print version string" )
            ( "help,h",    "Produce help message" );

        boost::program_options::options_description config( "Configuration" );
        config.add_options()
            ( "interface,i", boost::program_options::value< std::string >( &interface ),                                 "Interface or ip address" )
            ( "port,p",      boost::program_options::value< std::size_t >( &port )->default_value( port ),               "Port number" )
            ( "threads,t",   boost::program_options::value< std::size_t >( &thread_cnt )->default_value( thread_cnt ),   "Threads count" )
            ( "buffer,b",    boost::program_options::value< std::size_t >( &buffer_size )->default_value( buffer_size ), "Socket receiver buffer size" )
            ( "update,u",    boost::program_options::value< std::size_t >( &update_int )->default_value( update_int ),   "Update interval in seconds" );

        boost::program_options::options_description hidden( "Hidden options" );
        hidden.add_options()
            ( "input-file", boost::program_options::value< std::vector< std::string > >(), "input file" );

        boost::program_options::options_description cmdline_options;
        cmdline_options.add( generic ).add( config ).add( hidden );

        boost::program_options::options_description config_file_options;
        config_file_options.add( config ).add( hidden );

        boost::program_options::options_description visible;
        visible.add( generic ).add( config );

        boost::program_options::variables_map vm;
        boost::program_options::store( boost::program_options::command_line_parser( argc, argv ).options( cmdline_options ).run(), vm );
        boost::program_options::notify( vm );

        if ( vm.count( "help" ) || ( vm.count( "interface" ) == 0 ) ) {
            std::cout << visible << std::endl;
            return 1;
        }

        server srv( port, thread_cnt, buffer_size, update_int );
        srv.run();
    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
