#include "invertedIndex.h"

map<string, vector<Entry>> dictionary;

mutex readAccess;

void indexingFiles(vector<string> docs, size_t i) {
    stringstream ss(docs[i]);
    std::string word;
    unique_lock<mutex> lock(readAccess);
    while (ss >> word) {
        if (dictionary.find(word) == dictionary.end())
            dictionary[word].push_back({i, 1});
        else {
            bool findKey = false;
            for (auto &[key, value]: dictionary.find(word)->second)
                if (key == i) {
                    value++;
                    findKey = true;
                    break;
                }
            if (!findKey)
                dictionary[word].push_back({i, 1});
        }
    }
    lock.unlock();
}

void detachDocs(vector<string> docs){
    for (size_t i = 0; i < docs.size(); i++)
        thread(indexingFiles, docs, i).join();
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    thread(detachDocs, docs).join();
    lock_guard<mutex> lockGuard(readAccess);
    freq_dictionary = dictionary;

    for (auto &[key, value]: freq_dictionary) {
        sort(value.begin(), value.end(), [](const Entry &a, const Entry &b) {
            return a.doc_id < b.doc_id;
        });
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    return freq_dictionary[word];
}
