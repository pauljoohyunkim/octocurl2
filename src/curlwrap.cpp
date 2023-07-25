#include <curl/curl.h>
#include <iostream>
#include <stdexcept>
#include "curlwrap.hpp"

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

void curl_download(std::string url, std::string filename)
{
    auto smart_curl { smart_curl_constructor() };
    auto curl { (CURL*) smart_curl.get() };

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    CURLcode res { curl_easy_perform(curl) };

    if (res != CURLE_OK)
    {
        std::cerr << "Error while downloading from " << url << std::endl;
        std::cerr << curl_easy_strerror(res) << std::endl;
        return;
    }
}