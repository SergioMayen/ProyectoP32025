#include <iostream>
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// const string API_KEY = "AIzaSyC8ishIK-BIvWWHmgf5Gkt3rBKO39YEUYg";]
const string API_KEY = "key";

const string TARGET_LANG = "es";
const string TEXT_TO_TRANSLATE = "Hello";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

string extractTranslation(const string& jsonResponse) {
    json responseJson = json::parse(jsonResponse);
    return responseJson["data"]["translations"][0]["translatedText"];
}

string translateText(const string& text) {
    CURL* curl;
    CURLcode res;
    string response;

    string url = "https://translation.googleapis.com/language/translate/v2?key=" + API_KEY;
    string postData = "q=" + text + "&target=" + TARGET_LANG;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "cURL error: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    }
    return extractTranslation(response);;
}



int main() {

    string translatedText = translateText(TEXT_TO_TRANSLATE);
    cout << "Respuesta de Google Translate API:\n" << translatedText << endl;
    return 0;
}
