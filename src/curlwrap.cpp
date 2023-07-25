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