#include "curl_util.h"

std::string urlEncode(const std::string &input)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return "";

    char *encoded = curl_easy_escape(curl, input.c_str(), input.length());
    std::string encodedString = encoded ? std::string(encoded) : "";
    curl_free(encoded);
    curl_easy_cleanup(curl);

    return encodedString;
}

static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t totalSize = size * nmemb;
    output->append((char *)contents, totalSize);
    return totalSize;
}

std::string apiCall(CURL* hnd, std::string urlSuffix)
{
    std::string responseString;

    std::string url = "https://api.themoviedb.org/3/" + urlSuffix;

    if (!hnd)
    {
        std::cerr << "Failed to initialize cURL" << std::endl;
    }

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "accept: application/json");
    headers = curl_slist_append(headers, "Authorization: Bearer eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiJlOGY4ZTgzODEyZTFkODEyMTgyYTgyNTgwODcyNTAyZSIsIm5iZiI6MTczNzQ1MTQyMC40MTIsInN1YiI6IjY3OGY2NzljMDFhNzFhY2E1NGYwN2Y5ZSIsInNjb3BlcyI6WyJhcGlfcmVhZCJdLCJ2ZXJzaW9uIjoxfQ.w9g7uNm7YSgcf2ZaD4z6fIFhLnWJhF906-78hzgcV5s");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &responseString);

    CURLcode ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK)
    {
        std::cerr << "cURL request failed: " << curl_easy_strerror(ret) << std::endl;
    }
//     else
//     {
//         std::cout << "Response: " << responseString << std::endl;
//     }

    return responseString;
}