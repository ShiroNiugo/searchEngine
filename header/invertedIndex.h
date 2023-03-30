#include <utility>

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
            ss.str(docs[i]);
            while (getline(ss, word, ' ')) {
                if (!word.empty() && (freq_dictionary.find(word) == freq_dictionary.end())) {
                    freq_dictionary[word].push_back({i, 1});
                } else if (!word.empty() && (freq_dictionary.find(word) != freq_dictionary.end())) {
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
                }
                if (ss.eof()) break;
            }
            ss.clear();
        }
        //test
        /*for (auto &[keyWord, valueWord]: freq_dictionary) {
            cout << keyWord << ": " << endl;
            for (auto &value: valueWord)
                cout << "\t" << value.doc_id << ": " << value.count << endl;
        }*/
    }

    std::vector<Entry> GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs{};
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};

//TestInvertedIndexFunctionality
/*void TestInvertedIndexFunctionality
        (const vector<string> &docs, const vector<string> &requests, const std::vector<vector<Entry>> &expected) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (auto &request: requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {{{0, 1}},
                                            {{0, 1}, {1, 3}}};
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};
    const vector<vector<Entry>> expected = {
            {{0, 4}, {1, 1}, {2, 5}},
            {{0, 2}, {1, 2}, {2, 5}},
            {{3, 1}}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {},
            {{1, 1}}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}*/