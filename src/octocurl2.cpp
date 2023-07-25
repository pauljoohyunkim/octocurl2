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
    po::options_description generic("Generic options");
    generic.add_options()
        ("help,h", "produce help message")
        ("sort,s", "sort queue")
        ("urls", po::value<std::vector<std::string>>(), "url")
    ;
    po::positional_options_description p;
    p.add("urls", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(generic).positional(p).run(), vm);
    po::notify(vm);
    // For each option, do its action.
    if (vm.count("help"))
    {
        std::cout << generic << "\n";
        return 1;
    }
    if (vm.count("sort"))
    {
        octocurlOptions.sort = true;
        std::cout << "Sorting enabled.";
    }
    auto urls = vm["urls"].as<std::vector<std::string>>();
    for (auto url : urls)
    {
        std::cout << url << std::endl;
    }

    return EXIT_SUCCESS;
}
