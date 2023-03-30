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
    explicit SearchServer(InvertedIndex &idx) : _index(idx) {};

    vector<vector<RelativeIndex>> search(const vector<std::string> &queries_input) {
        vector<vector<RelativeIndex>> results;

        for (auto &query: queries_input) {
            map<size_t, size_t> absRelevance;
            float maxRelevance = 0;
            stringstream ss(query);
            string word;
            while (getline(ss, word, ' ')) {
                std::vector<Entry> temp = _index.GetWordCount(word);
                if (!word.empty() && !temp.empty()){
                    for (auto & t : temp){
                        absRelevance[t.doc_id] += t.count;
                        maxRelevance = max(maxRelevance, (float)absRelevance[t.doc_id]);
                    }
                }
                if(ss.eof()) break;
            }
            results.emplace_back();
            if (!absRelevance.empty()) {
                for (auto &it: absRelevance) {
                    string tempRank = to_string((float)it.second / maxRelevance);
                    tempRank.resize(3);
                    results.back().push_back({it.first, stof(tempRank)});
                }
            }
        }
        //sorting
        for(auto &r: results) {
            auto &tempToSort = r;
            sort(tempToSort.begin(), tempToSort.end(), [](const RelativeIndex &a, const RelativeIndex &b) {
                return a.rank > b.rank;
            });
        }
        //test
        /*int i = 0;
        for(auto & it : results) {
            printf("%i\n", i);
            for(auto & it2 : it) {
                printf("%zu %f\n", it2.doc_id, it2.rank);
            }
            i++;
        }*/

        return results;
    }

private:
    InvertedIndex _index;
};

//TestCaseSearchServer
/*TEST(TestCaseSearchServer, TestSimple) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> request = {"milk water", "sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {{2, 1},{0, 0.7},{1, 0.3}},
            {}
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.666666687},
                    {1, 0.666666687},
                    {2, 0.666666687}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}*/