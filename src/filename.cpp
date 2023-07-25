#include "filename.hpp"
#include "curlwrap.hpp"
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <string>

std::string getDefaultFilename(std::string url)
{
    auto smart_curl { smart_curl_constructor() };
    auto curl { (CURL*) smart_curl.get() };
    CURLcode res;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        res = curl_easy_perform(curl);
    }
    
    return std::string("asdf");
}