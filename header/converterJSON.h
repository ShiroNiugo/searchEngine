#pragma once

#define fileConfig "../config.json"
#define fileRequests "../requests.json"
#define fileAnswers "../answers.json"

class ConverterJSON {
public:
    ConverterJSON() = default;

    static std::vector<std::string> GetTextDocuments() {
        ifstream input(fileConfig);
        if (input.is_open()) {
            json file;
            input >> file;
            input.close();
            std::vector<std::string> textDocuments;
            for (const auto &document: file.find("files").value()) {
                input.open(document);
                if (input.is_open()) {
                    std::string word, words;
                    int countWords = 0;
                    while (input >> word) {
                        for(auto l : word)
                            if(!islower(l))
                                throw runtime_error("The words in the file do not follow the rules written in the instructions!");

                        words += (countWords != 0 ? " " : "") + ((word.size() <= 100) ? word : word.substr(0, 100));
                        countWords++;
                        if (countWords >= 1000) break;
                    }
                    textDocuments.push_back(words);
                    input.close();
                }else{
                    printf("%s file not found!\n", to_string(document).c_str());
                    textDocuments.emplace_back();
                }
            }
            return textDocuments;
        }
        return {};
    }

    static int GetResponsesLimit() {
        ifstream input(fileConfig);
        if (input.is_open()) {
            json temp;
            input >> temp;
            input.close();
            return temp.find("config").value().find("max_responses").value();
        }
        return 0;
    }

    static std::vector<std::string> GetRequests() {
        ifstream input(fileRequests);
        json tempRequests;
        input >> tempRequests;
        input.close();
        std::vector<std::string> tempValue = tempRequests.begin().value();
        if (tempValue.size() > 1000) tempValue.resize(1000);
        for (auto & item : tempValue) {
            stringstream ss(item);
            string word;
            for (int i = 0; getline(ss, word, ' '); i++) {
                if (i > 9 || ss.eof()) break;
                if (!word.empty()) {
                    for (auto l: word)
                        if (!islower(l))
                            throw runtime_error("The request does not comply with the rules written in the instructions!");
                }
            }
        }
        return tempValue;
    }

    static void putAnswers(const vector<vector<pair<int, float>>> &answers) {
        ifstream input(fileAnswers);
        json tempAnswers;
        input >> tempAnswers;
        input.close();
        auto sizeRequest = 1;
        for (const auto &one: answers) {
            json temp;
            std::string request = "request000";
            ((to_string(sizeRequest).length() < 3) ? request.erase(
                    request.length() - to_string(sizeRequest).length())
                                                   : request.erase(7));
            request += to_string(sizeRequest);
            sizeRequest++;
            if (one.empty()) {
                temp = {request, {{"result", "false"}}};
            } else {
                json t;
                for (const auto &[key, value]: one) {
                    t += {{"doc_id", key},
                          {"rank",   round((double) value * 1000) / 1000}};
                }
                temp = {request, {{"result", "true"}, {"relevance", t}}};
            }
            tempAnswers.begin().value().push_back(temp);
        }
        ofstream output(fileAnswers);
        output << tempAnswers;
        output.close();
    }

    static bool fileConfigVerify() {
        ifstream input(fileConfig);
        if (input.is_open()) {
            json temp;
            input >> temp;
            input.close();
            cout << "name: " << temp.find("config").value().find("name").value() << "\n" <<
                 "version " << temp.find("config").value().find("version").value() << endl;
            return true;
        } else {
            cout << "config.json file is missing!" << endl;
            return false;
        }
    }

    static void ClearFiles() {
        ofstream clearFile(fileAnswers);
        if (clearFile.is_open()) {
            clearFile << (json) {{"answers", {}}};
            clearFile.close();
        }
    }
};