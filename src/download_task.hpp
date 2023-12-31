#ifndef DOWNLOAD_TASK_HPP
#define DOWNLOAD_TASK_HPP

#include "default_params.hpp"
#include <algorithm>
#include <cstdio>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


struct DownloadTask
{
    DownloadTask(std::string url, std::string name) { file_url = url; file_name = name; }
    std::string file_url {};
    std::string file_name {};
    unsigned int file_size { 0 };
    unsigned int file_downloaded_length { 0 };
    bool downloaded { false };
    std::FILE* fp { nullptr };
};

struct OctocurlOptions
{
    bool sort { false };
};

bool task_one_before_task_two(DownloadTask task1, DownloadTask task2);

class OctocurlTaskManager
{
    public:
        void append(DownloadTask task) { tasks.push_back(task); }
        void sort() { std::sort(tasks.begin(), tasks.end(), task_one_before_task_two); }

        void setNumOfWorkers(unsigned int n) { num_of_workers = n; }

        // Prefetcher
        void launchPreworkers();
        // Downloader
        void launchWorkers();

        OctocurlOptions options;
    private:
        bool moreTasksLeft() { return pos < tasks.size(); }
        DownloadTask& operator [] (unsigned int n) { return tasks[n]; } 
        void prefetchWorker();
        void worker();
        void advance_in_tasks() { pos++; }
        void resetPos() { pos = 0; }

        std::vector<DownloadTask> tasks;
        unsigned int pos { 0 };
        unsigned int num_of_workers { DEFAULT_NUM_OF_WORKERS };
        std::mutex mtx;
        
};

#endif  // DOWNLOAD_TASK_HPP
