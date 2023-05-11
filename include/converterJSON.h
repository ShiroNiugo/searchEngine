#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

#define fileConfig "config.json"
#define fileRequests "requests.json"
#define fileAnswers "answers.json"

class ConverterJSON {
public:
    ConverterJSON() = default;

    static bool fileConfigVerify();

    static std::vector<std::string> GetTextDocuments();

    static int GetResponsesLimit();

    static std::vector<std::string> GetRequests();

    static void putAnswers(const vector<vector<pair<int, float>>> &answers);
};