#include "filename.hpp"
#include "curlwrap.hpp"
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <string>

std::string getDefaultFilename(std::string url)
{
    // If / found, get the filename directly from the url.
    auto pos { url.find_last_of('/') };
    if (pos != std::string::npos)
    {
        std::string filename { url.substr(pos+1) };
        return filename;
    }

    // Otherwise, do something more sophisticated by fetching header data.
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