#include "../src/filename.hpp"

int main()
{
    std::string url { "http://speedtest.ftp.otenet.gr/files/test1Mb.db" };
    auto filename { getDefaultFilename(url) };

    return 0;
}