#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

#include "../header/converterJSON.h"
#include "../header/invertedIndex.h"
#include "../header/searchServer.h"

int main() {
    InvertedIndex invIndex;
    if (!ConverterJSON::fileConfigVerify())
        return 0;
    invIndex.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
    vector<std::string> requests = ConverterJSON::GetRequests();

    SearchServer server(invIndex);
    auto result = [&server, &requests](vector<vector<pair<int, float>>> answers = {}){
        auto foo = server.search(requests);
        for (auto const &relIndex: foo) {
            answers.emplace_back();
            int limit = ConverterJSON::GetResponsesLimit();
            for (auto request: relIndex) {
                answers.back().emplace_back(request.doc_id, request.rank);
                limit--;
                if (limit == 0) break;
            }
        }
        return answers;
    };
    ConverterJSON::ClearFiles();
    ConverterJSON::putAnswers(result());
    return 0;
}