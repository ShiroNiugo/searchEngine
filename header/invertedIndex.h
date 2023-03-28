#pragma once

using namespace std;

struct Entry {
    size_t doc_id;
    size_t count;

    bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs) {
        docs = input_docs;
        for (size_t i = 0; i < docs.size(); i++) {
            if (!docs[i].empty()) {
                stringstream ss(docs[i]);
                std::string word;
                ss >> word;
                if (freq_dictionary[word].size() == 5) break;
                if(freq_dictionary.find(word) != freq_dictionary.end())
                    freq_dictionary[word].push_back({i, 1});
                else
                    freq_dictionary[word][i].count++;
                std::cout << "+" << std::endl;//test
            }
        }
    }

    std::vector<Entry> GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};