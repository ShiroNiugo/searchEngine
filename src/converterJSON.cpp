#include "converterJSON.h"

using namespace std;

bool ConverterJSON::fileConfigVerify() {
    if (ifstream input(fileConfig); input.is_open()) {
        json temp;
        input >> temp;
        input.close();
        cout << "name: " << temp.find("config").value().find("name").value() << "\n" <<
             "version " << temp.find("config").value().find("version").value() << endl;
        return true;
    }
    cout << "config.json file is missing!" << endl;
    return false;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    if (ifstream input(fileConfig); input.is_open()) {
        json file;
        input >> file;
        input.close();
        std::vector<std::string> textDocuments;
        for (auto const &document: file.find("files").value()) {
            input.open(document);
            if (input.is_open()) {
                std::string word;
                std::string words;
                int countWords = 0;
                while (input >> word) {
                    for (auto l: word)
                        if (!islower(l))
                            throw invalid_argument(
                                    "The words in the file do not follow the rules written in the instructions!");

                    words += (countWords != 0 ? " " : "") + ((word.size() <= 100) ? word : word.substr(0, 100));
                    countWords++;
                    if (countWords >= 1000) break;
                }
                textDocuments.push_back(words);
                input.close();
            } else {
                printf("%s file not found!\n", to_string(document).c_str());
                textDocuments.emplace_back();
            }
        }
        return textDocuments;
    }
    return {};
}

int ConverterJSON::GetResponsesLimit() {
    if (ifstream input(fileConfig); input.is_open()) {
        json temp;
        input >> temp;
        input.close();
        return temp.find("config").value().find("max_responses").value();
    }
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    ifstream input(fileRequests);
    json tempRequests;
    input >> tempRequests;
    input.close();
    std::vector<std::string> tempValue = tempRequests.begin().value();
    if (tempValue.size() > 1000) tempValue.resize(1000);
    for (auto const &item: tempValue) {
        stringstream ss(item);
        string word;
        int i = 0;
        while(getline(ss, word, ' ')) {
            if (i > 9 || ss.eof()) break;
            if (!word.empty()) {
                for(auto l: word)
                    if (!islower(l))
                        throw std::invalid_argument("The request does not comply with the rules written in the instructions!");
            }
            i++;
        }
    }
    return tempValue;
}

void ConverterJSON::putAnswers(const vector<vector<pair<int, float>>> &answers) {
    if (!answers.empty()) {
        int sizeRequest = 1;
        json tempAnswers;
        for (auto &block: answers) {
            std::string request = "request000";
            ((to_string(sizeRequest).length() < 3) ? request.erase(
                    request.length() - to_string(sizeRequest).length()) : request.erase(7));
            request += to_string(sizeRequest);
            sizeRequest++;
            if (block.empty()) tempAnswers.emplace(request, (json) {{"result", "false"}});
            else if (block.size() == 1)
                tempAnswers.emplace(request, (json) {{"result", "true"},
                                                     {"docId",  block.begin()->first},
                                                     {"rank",   round((double) block.begin()->second * 1000) / 1000}});
            else {
                json t;
                for(auto &[key, value]: block)
                    t += {{"docId", key},
                          {"rank",  round((double) value * 1000) / 1000}};
                tempAnswers.emplace(request, (json) {{"result", "true"}, {"relevance", t}});
            }
        }
        ofstream output(fileAnswers);
        if(output.is_open()) {
            cout << "Result: answers to " << answers.size() << " queries were found" << endl;
            output << (json) {{"answers", tempAnswers.empty() ? (json) {} : tempAnswers}};
            output.close();
        }else {cout << "The file \"answers.json\" does not open" << endl;}
    } else cout << "Result: no answers to the queries were found" << endl;
}