#ifndef CURLWRAP_HPP
#define CURLWRAP_HPP

#include <curl/curl.h>
#include <memory>

std::unique_ptr<CURL, void(*)(CURL*)> smart_curl_constructor();
void smart_curl_deleter(CURL* curl);
void curl_download(std::string url, std::string filename);

#endif