#include "download_task.hpp"
#include "curlwrap.hpp"
#include <thread>
#include <vector>

bool task_one_before_task_two(DownloadTask task1, DownloadTask task2)
{
    return (task1.file_size > task2.file_size);
}

void OctocurlTaskManager::launchPreworkers()
{
    std::vector<std::thread> preworkers {};
    for (unsigned int i = 0; i < num_of_workers; i++)
    {
        std::thread th { &OctocurlTaskManager::prefetchWorker, this };
        preworkers.push_back(std::move(th));
    }
    for (unsigned int i = 0; i < num_of_workers; i++)
    {
        preworkers[i].join();
    }
    resetPos();

    if (options.sort)
    {
        sort();
    }
}

void OctocurlTaskManager::launchWorkers()
{
    std::vector<std::thread> workers;
    for (unsigned int i = 0; i < num_of_workers; i++)
    {
        std::thread th { &OctocurlTaskManager::worker, this };
        workers.push_back(std::move(th));
    }
    for (unsigned int i = 0; i < num_of_workers; i++)
    {
        workers[i].join();
    }
}

// Prefetching file size.
void OctocurlTaskManager::prefetchWorker()
{
    while (true)
    {
        DownloadTask *ptask;
        mtx.lock();
        if (moreTasksLeft())
        {
            ptask = &tasks[pos];
            advance_in_tasks();
        }
        else
        {
            mtx.unlock();
            break;
        }
        mtx.unlock();

        // Do the prefetch
        DownloadTask& task { *ptask };
        curl_prefetch_filesize(task);
    }
}

void OctocurlTaskManager::worker()
{
    while (true)
    {
        // Assign task to work with
        DownloadTask *ptask;
        mtx.lock();
        if (moreTasksLeft())
        {
            ptask = &tasks[pos];
            advance_in_tasks();
        }
        else
        {
            mtx.unlock();
            break;
        }
        mtx.unlock();

        // Do the task
        DownloadTask& task { *ptask };
        curl_download(task);
    }
}