#include "filename.hpp"
#include "curlwrap.hpp"
#include <curl/curl.h>
#include <memory>
#include <string>

std::string getDefaultFilename(std::string url)
{
    auto smart_curl { smart_curl_constructor() };
    
    return std::string("asdf");
}