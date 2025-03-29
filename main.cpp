#include <iostream>
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

const string API_KEY = "AIzaSyC8ishIK-BIvWWHmgf5Gkt3rBKO39YEUYg";
struct Traducciones {
    vector<string> targets = {"es","it", "fr", "de", "en"};
};

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
    Traducciones tr;
    CURL* curl;
    CURLcode res;
    string translatedText = "";

    string url = "https://translation.googleapis.com/language/translate/v2?key=" + API_KEY;

    for (const auto& target : tr.targets) {
        string response;
        string postData = "q=" + text + "&target=" + target;

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
            } else {
                translatedText += "Traducción a " + target + ": " + extractTranslation(response) + "\n";
            }

            curl_easy_cleanup(curl);
        }
    }

    return translatedText;
}

int main() {
    string textEntry;

    cout << "Ingrese el texto a traducir:\n";
    getline(cin, textEntry);

    string translatedText = translateText(textEntry);
    cout << "Respuestas de Google Translate API:\n" << translatedText << endl;
    return 0;
}

