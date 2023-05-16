#include "invertedIndex.h"

using namespace std;

std::mutex readAccess;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);

    auto indexingFiles = [this](const string& doc, size_t i) {
        stringstream ss(doc);
        std::string word;
        scoped_lock<mutex> lockGuard(readAccess);
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
    };

    for (size_t i = 0; i < docs.size(); i++)
        thread(indexingFiles, docs[i], i).join();

    scoped_lock<mutex> lockGuard(readAccess);
    for (auto &[key, value]: freq_dictionary) {
        sort(value.begin(), value.end(), [](const Entry &a, const Entry &b) {
            return a.doc_id < b.doc_id;
        });
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    return freq_dictionary[word];
}
