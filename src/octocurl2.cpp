#include <boost/program_options.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "download_task.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    OctocurlOptions octocurlOptions;
    /*
     * Parsing command line options
     * --help, -h: Help Message
     * --sort, -s: Sort Queue
    */
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("sort,s", "sort queue")
        ("urls", po::value<std::vector<std::string>>(), "url")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }
    if (vm.count("sort"))
    {
        octocurlOptions.sort = true;
        std::cout << "Sorting enabled.";
    }
    std::cout << vm["urls"].as<std::vector<std::string>>()[0];

    return EXIT_SUCCESS;
}
