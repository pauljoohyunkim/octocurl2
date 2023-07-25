#ifndef CURLWRAP_HPP
#define CURLWRAP_HPP

#include <curl/curl.h>
#include <memory>
#include "download_task.hpp"

std::unique_ptr<CURL, void(*)(CURL*)> smart_curl_constructor();
void smart_curl_deleter(CURL* curl);
void curl_download(DownloadTask task);

#endif