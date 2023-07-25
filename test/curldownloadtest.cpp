#include "../src/curlwrap.hpp"
#include "../src/download_task.hpp"
#include <string>

int main()
{
    std::string url { "https://raw.githubusercontent.com/github/gitignore/main/C%2B%2B.gitignore" };
    std::string filename { "gitignore_test" };
    auto task { DownloadTask(url, filename) };
    curl_prefetch_filesize(task);
    curl_download(task);

    return 0;
}