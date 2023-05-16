#include "searchServer.h"

using namespace std;

std::mutex access;

vector <vector<RelativeIndex>> SearchServer::search(const vector<std::string> &queries_input) {
    vector<vector<RelativeIndex>> results;

    auto indexDocuments = [this, &results](string const& query) {
        map<size_t, size_t> absRelevance;
        float maxRelevance = 0;
        stringstream ss(query);
        string word;
        while (ss >> word) {
            std::vector<Entry> temp = index.GetWordCount(word);
            if (!word.empty() && !temp.empty()) {
                for(auto const &t: temp) {
                    absRelevance[t.doc_id] += t.count;
                    maxRelevance = max(maxRelevance, (float)absRelevance[t.doc_id]);
                }
            }
        }
        scoped_lock<mutex> lockGuard(access);
        results.emplace_back();
        if (!absRelevance.empty())
            for(auto const &[key, value]: absRelevance)
                results.back().push_back({key, (float) value / maxRelevance});
    };

    for(auto query : queries_input)
        thread(indexDocuments, query).join();

    scoped_lock<mutex> lockGuard(access);
    for (auto &block: results) {
        auto &tempToSort = block;
        sort(tempToSort.begin(), tempToSort.end(), [](const RelativeIndex &a, const RelativeIndex &b) {
            return a.rank > b.rank || (a.rank == b.rank && a.doc_id < b.doc_id);
        });
    }
    for (auto &block: results)
        block.resize(block.size() > 5 ? ConverterJSON::GetResponsesLimit() : block.size());
    return results;
}