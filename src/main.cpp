#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <thread>
#include <mutex>

#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

#include "../header/converterJSON.h"
#include "../header/invertedIndex.h"
#include "../header/searchServer.h"

//#include "testGtest.h"

int main() {
    try {
        if (!ConverterJSON::fileConfigVerify())
            return 0;
        InvertedIndex invIndex;
        invIndex.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
        vector<std::string> requests = ConverterJSON::GetRequests();

        SearchServer server(invIndex);
        auto result = [&server, &requests](vector<vector<pair<int, float>>> answers = {}) {
            auto foo = server.search(requests);
            for (auto const &relIndex: foo) {
                answers.emplace_back();
                for (auto request: relIndex)
                    answers.back().emplace_back(request.doc_id, request.rank);
            }
            return answers;
        };
        ConverterJSON::ClearFiles();
        ConverterJSON::putAnswers(result());
    } catch (const runtime_error &error) {
        cerr << error.what() << endl;
    }
    return 0;
}