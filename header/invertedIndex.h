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
        docs = input_docs;
        for (size_t i = 0; i < docs.size(); i++) {
            if (!docs[i].empty()) {
                stringstream ss(docs[i]);
                std::string word;
                while(getline(ss, word, ' ')){
                    if(freq_dictionary.find(word) != freq_dictionary.end())
                        freq_dictionary[word].push_back({i, 1});
                    else
                        freq_dictionary[word][i].count++;
                }
            }
        }
        sort(freq_dictionary.begin()->second.begin(), freq_dictionary.end()->second.end(), [](Entry &a, Entry &b){
            return a.count < b.count;
        });
        for(auto &[keyWord, valueWord] : freq_dictionary){
            cout << keyWord << ": " << endl;
            for(auto &[key, value] : valueWord){
                cout << "\t" << key << ": " << value << endl;
            }
        }
    }

    std::vector<Entry> GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }
private:
    std::vector<std::string> docs{};
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};