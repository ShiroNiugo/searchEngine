#pragma once

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    explicit SearchServer(InvertedIndex &idx) : index(idx) {};

    vector<vector<RelativeIndex>> search(const vector<std::string> &queries_input) {
        vector<vector<RelativeIndex>> results;

        for (auto &query: queries_input) {
            map<size_t, size_t> absRelevance;
            float maxRelevance = 0;
            stringstream ss(query);
            string word;
            while (ss >> word) {
                std::vector<Entry> temp = index.GetWordCount(word);
                if (!word.empty() && !temp.empty()){
                    for (auto & t : temp){
                        absRelevance[t.doc_id] += t.count;
                        maxRelevance = max(maxRelevance, (float)absRelevance[t.doc_id]);
                    }
                }
            }
            results.emplace_back();
            if (!absRelevance.empty())
                for (auto &[key, value]: absRelevance)
                    results.back().push_back({key, (float)value / maxRelevance});
        }
        for(auto &block: results) {
            auto &tempToSort = block;
            sort(tempToSort.begin(), tempToSort.end(), [](const RelativeIndex &a, const RelativeIndex &b) {
                return a.rank > b.rank || (a.rank == b.rank && a.doc_id < b.doc_id);
            });
        }
        for(auto &block: results)
            block.resize(block.size()>5 ? ConverterJSON::GetResponsesLimit() : block.size());
        return results;
    }

private:
    InvertedIndex index;
};