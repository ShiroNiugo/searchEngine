#pragma once

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
        docs = std::move(input_docs);
        stringstream ss;
        std::string word;
        for (size_t i = 0; i < docs.size(); i++) {
            if(!docs[i].empty()) {
                ss.str(docs[i]);
                while (ss >> word) {
                    if (freq_dictionary.find(word) == freq_dictionary.end()) {
                        freq_dictionary[word].push_back({i, 1});
                    } else {
                        bool findKey = false;
                        for(auto &[key, value] : freq_dictionary.find(word)->second)
                            if(key == i) { value++; findKey = true; break;}
                        if(!findKey)
                            freq_dictionary[word].push_back({i, 1});
                    }
                    if (ss.eof()) break;
                }
                ss.clear();
            }
        }
        for(auto &[key, value]: freq_dictionary) {
            sort(value.begin(), value.end(), [](const Entry &a, const Entry &b) {
                return a.count < b.count;
            });
        }
    }

    std::vector<Entry> GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs{};
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};