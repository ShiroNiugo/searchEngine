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
        stringstream ss;
        std::string word;
        for (size_t i = 0; i < docs.size(); i++) {
            ss.str(docs[i].c_str());
            while (getline(ss, word, ' ')) {
                if (!word.empty()) {
                    if (freq_dictionary.find(word) != freq_dictionary.end()) {
                        auto freq = freq_dictionary.find(word)->second;
                        if (!freq.empty()) {
                            auto it = find_if(begin(freq), end(freq), [i](const Entry &e) {
                                return e.doc_id == i;
                            });
                            if (it != end(freq))
                                freq_dictionary[word][i].count++;
                            else
                                freq_dictionary[word].push_back({i, 1});
                        } else
                            freq_dictionary[word].push_back({i, 1});
                    } else
                        freq_dictionary[word].push_back({i, 1});
                }
                if (ss.eof()) break;
            }
            ss.clear();
        }

        for (auto &[keyWord, valueWord]: freq_dictionary) {
            cout << keyWord << ": " << endl;
            for (auto it = valueWord.begin(); it != valueWord.end(); ++it)
                cout << "\t" << it->doc_id << ": " << it->count << endl;
        }

        printf("sort freq_dictionary => +\n");//test
        sort(freq_dictionary.begin()->second.begin(), freq_dictionary.end()->second.end(), [](Entry &a, Entry &b) {
            return a.count < b.count;
        });
        printf("sort end freq_dictionary => +\n");//test
        /* */
        for (auto &[keyWord, valueWord]: freq_dictionary) {
            cout << keyWord << ": " << endl;
            for (auto it = valueWord.begin(); it != valueWord.end(); ++it)
                cout << "\t" << it->doc_id << ": " << it->count << endl;
        }
    }

    std::vector<Entry> GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs{};
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};