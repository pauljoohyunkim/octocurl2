#include <boost/program_options.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "download_task.hpp"

namespace po = boost::program_options;

static void parseCommandLineArguments(int argc, char** argv, OctocurlTaskManager& tm);

int main(int argc, char** argv)
{
    OctocurlTaskManager taskManager;
    /*
     * Parsing command line options
     * --help, -h: Help Message
     * --sort, -s: Sort Queue
    */
    parseCommandLineArguments(argc, argv, taskManager);

    return EXIT_SUCCESS;
}

static void parseCommandLineArguments(int argc, char** argv, OctocurlTaskManager& tm)
{
    po::options_description generic("Generic options");
    generic.add_options()
        ("help,h", "produce help message")
        ("sort,s", "sort queue")
    ;
    po::options_description hidden;
    hidden.add_options()
        ("urls", po::value<std::vector<std::string>>(), "url")
    ;

    po::options_description composite;
    composite.add(generic).add(hidden);
    po::positional_options_description p;
    p.add("urls", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(composite).positional(p).run(), vm);
    po::notify(vm);
    // For each option, do its action.
    if (vm.count("help"))
    {
        std::cout << generic << "\n";
        exit(1);
    }
    if (vm.count("sort"))
    {
        tm.options.sort = true;
        std::cout << "Sorting enabled." << std::endl;
    }
    if (vm.count("urls"))
    {
        auto urls = vm["urls"].as<std::vector<std::string>>();
        for (auto url : urls)
        {
            std::cout << url << std::endl;
        }
    }

}