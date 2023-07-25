#include "../src/curlwrap.hpp"
#include <string>

int main()
{
    std::string url { "https://raw.githubusercontent.com/github/gitignore/main/C%2B%2B.gitignore" };
    curl_download(url, "gitignore_test");

    return 0;
}