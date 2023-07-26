#include <boost/program_options.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <regex>
#include "curlwrap.hpp"
#include "default_params.hpp"
#include "download_task.hpp"
#include "filename.hpp"
#include "octocurl2.hpp"

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
     * --nwork, -n: Number of workers
    */
    parseCommandLineArguments(argc, argv, taskManager);
    taskManager.launchPreworkers();
    taskManager.launchWorkers();

    return EXIT_SUCCESS;
}

static void parseCommandLineArguments(int argc, char** argv, OctocurlTaskManager& tm)
{
    po::options_description generic("Generic options");
    generic.add_options()
        ("help,h", "produce help message")
        ("version,v", "print version")
    ;
    po::options_description usage("Usage options");
    std::string numOfWorkersDescriptionString { "number of workers (default: "};
    numOfWorkersDescriptionString += std::to_string(DEFAULT_NUM_OF_WORKERS);
    numOfWorkersDescriptionString += ")";
    usage.add_options()
        ("sort,s", "sort queue after prefetching file size.")
        ("nwork,n", po::value<unsigned int>(), numOfWorkersDescriptionString.c_str())
    ;
    po::options_description hidden;
    hidden.add_options()
        ("inputs", po::value<std::vector<std::string>>(), "inputs")
    ;

    po::options_description composite;
    composite.add(generic).add(usage).add(hidden);
    po::positional_options_description p;
    p.add("inputs", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(composite).positional(p).run(), vm);
    po::notify(vm);
    // For each option, do its action.
    if (vm.count("help"))
    {
        std::cout << "Usage: " << argv[0] << " [options] url[::output filename] [url[::output filename]] ..." << std::endl;
        std::cout << usage << generic << "\n";
        exit(1);
    }
    if (vm.count("version"))
    {
        std::cout << "Version: " << OCTOCURL_VERSION << std::endl;
        exit(1);
    }
    if (vm.count("sort"))
    {
        tm.options.sort = true;
        std::cout << "Sorting enabled." << std::endl;
    }
    if (vm.count("nwork"))
    {
        unsigned int nWorkers { vm["nwork"].as<unsigned int>() };
        tm.setNumOfWorkers(nWorkers);
        std::cout << "Number of workers set to " << nWorkers << std::endl;
    }
    if (vm.count("inputs"))
    {
        auto inputs = vm["inputs"].as<std::vector<std::string>>();
        for (auto inputstring : inputs)
        {
            std::string url {};
            std::string filename {};
            detectOutputName(inputstring, url, filename);
            DownloadTask task { url, filename };
            tm.append(task);
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