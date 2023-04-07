#pragma once

struct Entry {
    size_t doc_id;
    size_t count;

    bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

std::map<std::string, std::vector<Entry>> freq_dictionary;
int countActiveThread = 0;
mutex readAccess;

void indexingFiles(const string &document, const size_t &i) {
    stringstream ss(document);
    std::string word;
    unique_lock<mutex> lock(readAccess);
    while (ss >> word) {
        if (freq_dictionary.find(word) == freq_dictionary.end())
            freq_dictionary[word].push_back({i, 1});
        else {
            bool findKey = false;
            for (auto &[key, value]: freq_dictionary.find(word)->second)
                if (key == i) {
                    value++;
                    findKey = true;
                    break;
                }
            if (!findKey)
                freq_dictionary[word].push_back({i, 1});
        }
    }
    lock.unlock();
    countActiveThread--;
}

class InvertedIndex {
private:
    std::vector<std::string> docs;
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs) {
        docs = std::move(input_docs);
        for (size_t i = 0; i < docs.size(); i++) {
            countActiveThread++;
            thread(indexingFiles, docs[i], i).detach();
        }
        while (countActiveThread != 0);
        lock_guard<mutex> lockGuard(readAccess);
        for (auto &[key, value]: freq_dictionary) {
            sort(value.begin(), value.end(), [](const Entry &a, const Entry &b) {
                return a.doc_id < b.doc_id;
            });
        }
    }

    std::vector<Entry> GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }
};