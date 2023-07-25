#include "download_task.hpp"

bool task_one_before_task_two(DownloadTask task1, DownloadTask task2)
{
    return (task1.file_size > task2.file_size);
}