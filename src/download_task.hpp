#ifndef DOWNLOAD_TASK_HPP
#define DOWNLOAD_TASK_HPP

#include <string>
#include <vector>

struct DownloadTask
{
    DownloadTask(std::string url, std::string name) { file_url = url; file_name = name; }
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

class OctocurlTaskManager
{
    public:
        void append(DownloadTask task) { tasks.push_back(task); }
        DownloadTask& operator [] (unsigned int n) { return tasks[n]; } 

        OctocurlOptions options;
    private:
        std::vector<DownloadTask> tasks;
};

#endif  // DOWNLOAD_TASK_HPP
