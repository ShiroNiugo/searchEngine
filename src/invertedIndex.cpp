#include "invertedIndex.h"

using namespace std;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    std::mutex readAccess;

    auto indexingFiles = [this, &readAccess](const string& doc, size_t index) {
        stringstream ss(doc);
        std::string word;
        scoped_lock<mutex> guard(readAccess);
        while (ss >> word) {
            bool noIterator = true;
            if (auto findWord = freq_dictionary.find(word); findWord != freq_dictionary.end()) {
                auto value = &findWord->second;
                auto temp = find_if(value->begin(), value->end(), [&index](const Entry &a) {
                    return a.doc_id == index;
                });
                if(temp != value->end()) {
                    temp->count++;
                    noIterator = false;
                }
            }
            if(noIterator) freq_dictionary[word].push_back({index, 1});
        }
    };

    vector<thread> tempThread(docs.size());
    int i = 0;
    for (auto doc : docs) {
        tempThread[i] = thread(indexingFiles, doc, i);
        i++;
    }

    for (auto &t : tempThread)
        t.join();

    scoped_lock<mutex> guard(readAccess);
    for (auto &[key, value]: freq_dictionary) {
        sort(value.begin(), value.end(), [](const Entry &a, const Entry &b) {
            return a.doc_id < b.doc_id;
        });
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    return freq_dictionary[word];
}
