#pragma once

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex &idx) : _index(idx) {};

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input){
        std::vector<std::vector<RelativeIndex>> results{};
        //split and count words search
        for (auto query : queries_input){
            auto temp = _index.GetWordCount(query);
            if(!temp.empty()){
                results.push_back({});
            }
        }
        //sorting
        return results;
    }

private:
    InvertedIndex _index;
};