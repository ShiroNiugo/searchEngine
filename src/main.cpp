#include "converterJSON.h"
#include "invertedIndex.h"
#include "searchServer.h"

using namespace std;

int main() {
    if (!ConverterJSON::fileConfigVerify()) return 0;
    try {
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
        ConverterJSON::putAnswers(result());
    } catch (const invalid_argument &error) {
        cerr << error.what() << endl;
    }
    return 0;
}