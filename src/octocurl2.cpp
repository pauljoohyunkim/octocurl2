#include <boost/program_options.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <regex>
#include "download_task.hpp"
#include "filename.hpp"

namespace po = boost::program_options;

static void parseCommandLineArguments(int argc, char** argv, OctocurlTaskManager& tm);
static void detectOutputName(std::string string, std::string& url, std::string& filename, std::string delimiter="::");

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
        ("inputs", po::value<std::vector<std::string>>(), "inputs")
    ;

    po::options_description composite;
    composite.add(generic).add(hidden);
    po::positional_options_description p;
    p.add("inputs", -1);
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
    if (vm.count("inputs"))
    {
        auto inputs = vm["inputs"].as<std::vector<std::string>>();
        for (auto inputstring : inputs)
        {
            std::string url {};
            std::string filename {};
            detectOutputName(inputstring, url, filename);
            tm.append(DownloadTask(url, filename));
        }
    }
}

static void detectOutputName(std::string string, std::string& url, std::string& filename, std::string delimiter)
{
    std::regex filter { delimiter };
    std::smatch sm;
    std::regex_search(string, sm, filter);
    if (sm.empty() == false)
    {
        auto pos { sm.position(0) };
        url = string.substr(0, pos);
        filename = string.substr(pos+delimiter.length());
    }
    else
    {
        url = string;
        filename = getDefaultFilename(url);
    }
}