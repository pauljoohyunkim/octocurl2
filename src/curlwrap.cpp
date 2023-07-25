#include <cstdio>
#include <curl/curl.h>
#include <iostream>
#include <stdexcept>
#include "curlwrap.hpp"
#include "download_task.hpp"

static size_t get_data_through_task(char* buffer, size_t itemsize, size_t nitems, void* ptr);

std::unique_ptr<CURL, void(*)(CURL*)> smart_curl_constructor()
{
    CURL* curl { curl_easy_init() };

    if (!curl)
    {
        throw std::runtime_error("CURL failed to initialize!");
    }
    auto smart_curl_pointer = std::unique_ptr<CURL, void (*)(CURL *)>(curl, smart_curl_deleter);
    return smart_curl_pointer;
}

void smart_curl_deleter(CURL* curl)
{
    curl_easy_cleanup(curl);
    std::cerr << "smart_curl_deleter called" << std::endl;
}

void curl_download(DownloadTask& task)
{
    auto smart_curl { smart_curl_constructor() };
    auto curl { (CURL*) smart_curl.get() };
    auto url { task.file_url };
    auto filename { task.file_name };
    task.fp = std::fopen(filename.c_str(), "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data_through_task);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &task);
    CURLcode res { curl_easy_perform(curl) };

    if (res != CURLE_OK)
    {
        std::cerr << "Error while downloading from " << url << std::endl;
        std::cerr << curl_easy_strerror(res) << std::endl;
        goto cleanup;
    }

    task.downloaded = true;

    cleanup:
        std::fclose(task.fp);
}

void curl_prefetch_filesize(DownloadTask& task)
{
    auto smart_curl { smart_curl_constructor() };
    auto curl { (CURL*) smart_curl.get() };

    // Only get the file size data not the file content.
    curl_easy_setopt(curl, CURLOPT_URL, task.file_url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
    auto res { curl_easy_perform(curl) };

    if (res != CURLE_OK)
    {
        std::cerr << "Could not fetch file info from " << task.file_url << std::endl;
        return;
    }

    curl_off_t filelength;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &filelength);

    task.file_size = filelength;
}

static size_t get_data_through_task(char* buffer, size_t itemsize, size_t nitems, void* ptr)
{
    size_t bytes { itemsize * nitems };
    auto ptask { (DownloadTask*) ptr };
    std::fwrite(buffer, itemsize, nitems, ptask->fp);
    
    return bytes;
}