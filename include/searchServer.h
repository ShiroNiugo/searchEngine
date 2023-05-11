#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "invertedIndex.h"
#include "converterJSON.h"

using namespace std;

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    explicit SearchServer(InvertedIndex &idx) : index(idx) {};

    vector<vector<RelativeIndex>> search(const vector<std::string> &queries_input);

private:
    InvertedIndex index;
};