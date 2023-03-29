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
    SearchServer(InvertedIndex &idx) : _index(idx) {};

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input) {
        std::vector<std::vector<RelativeIndex>> results;
        //split and count words search
        for (auto &query: queries_input) {
            map<size_t, size_t> absRelevance;
            stringstream ss(query);
            string word;
            while (getline(ss, word, ' ')) {
                std::vector<Entry> temp = _index.GetWordCount(word);
                if (!temp.empty())
                    for (auto it = temp.begin(); it != temp.end(); it++)
                        absRelevance[it->doc_id] += it->count;
            }
            if (!absRelevance.empty()) {
                int maxCount = max_element(absRelevance.begin(), absRelevance.end())->second;
                for (auto it = absRelevance.begin(); it != absRelevance.end(); it++)
                    results.push_back({{it->first, ((float) it->second / (float) maxCount)}});
            } else {
                results.push_back({});
            }
        }
        //sorting
        return results;
    }

private:
    InvertedIndex _index;
};