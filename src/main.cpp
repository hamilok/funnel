#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

#include "server.hpp"

int main(int argc, char* argv[])
{
    try
    {
        std::string interface;
        int port = 0;

        boost::program_options::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "print version string")
            ("help,h", "produce help message");

        boost::program_options::options_description config("Configuration");
        config.add_options()
            ("interface,i", boost::program_options::value<std::string>(&interface), "interface or ip address")
            ("port,p", boost::program_options::value<int>(&port)->default_value(9500), "port number");

        boost::program_options::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", boost::program_options::value< std::vector<std::string> >(), "input file");

        boost::program_options::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        boost::program_options::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        boost::program_options::options_description visible("Allowed options");
        visible.add(generic).add(config);

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
        boost::program_options::notify(vm);

        if (vm.count("help") || (vm.count("interface") == 0 || vm.count("port") == 0)) {
            std::cout << visible << std::endl;
            return 1;
        }

        server srv(port);
        srv.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
