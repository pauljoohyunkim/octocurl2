#ifndef DOWNLOAD_TASK_HPP
#define DOWNLOAD_TASK_HPP

#include <string>

struct DownloadTask
{
    DownloadTask(std::string url, std::string name);
    std::string file_url {};
    std::string file_name {};
    unsigned int file_size { 0 };
    unsigned int file_downloaded_length { 0 };
    bool downloaded { false };
};

struct OctocurlOptions
{
    bool sort { false };
};

#endif  // DOWNLOAD_TASK_HPP
