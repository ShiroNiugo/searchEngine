#pragma once

#define fileConfig "../config.json"
#define fileRequests "../requests.json"
#define fileAnswers "../answers.json"


using json = nlohmann::json;

using namespace std;

class ConverterJSON {
public:
    ConverterJSON() = default;

    ~ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments() {
        ifstream input(fileConfig);
        if (input.is_open()) {
            json file;
            input >> file;
            input.close();
            std::vector<std::string> textDocuments;
            for (auto document: file.find("files").value()) {
                input.open(document);
                if (input.is_open()) {
                    std::string words;
                    int countWords = 0;
                    while(!input.eof()) {
                        std::string word;
                        input >> word;
                        words.append(" "+((word.size()<=100)? word : word.substr(0,100)));
                        countWords++;
                        if(countWords == 1000) break;
                    }
                    textDocuments.push_back(words);
                    input.close();
                }
            }
            for (const auto &document: textDocuments)
                cout << document << endl;
            return textDocuments;
        }
        return {};
    }

    int GetResponsesLimit() {
        ifstream input(fileConfig);
        if (input.is_open()) {
            json temp;
            input >> temp;
            input.close();
            return temp.find("config").value().find("max_responses").value();
        }
        return 0;
    }

    std::vector<std::string> GetRequests() {
        ifstream input(fileRequests);
        if (input.is_open()) {
            json temp;
            input >> temp;
            input.close();
            return temp.begin().value();
        }
        return {};
    }

    void putAnswers(std::vector<std::vector<std::pair<size_t, float>>> answers) {
        ifstream input(fileAnswers);
        json tempAnswers;
        input >> tempAnswers;
        input.close();
        auto sizeRequest = (int) tempAnswers.begin().value().size();
        auto sizeAnswers = (int) answers.size();
        json t;

        for (auto one: answers)
            for (auto &[key, value]: one)
                t += {{"docid", key},
                      {"rank",  value}};

        std::string request = "request" + (std::string) ((to_string(sizeRequest).length() == 1) ? "00" : (to_string(
                sizeRequest).length() == 2) ? "0" : "") + to_string(sizeRequest == 0 ? 1 : sizeRequest + 1);

        json temp;
        if (sizeAnswers > 0) {
            temp = {
                    {request, {
                            {"result", "true"},
                            {"relevance", t}
                    }}
            };
        } else {
            temp = {
                    {request, {
                            {"result", "false"}
                    }}
            };
        }
        ofstream output(fileAnswers);
        tempAnswers.begin().value().push_back(json::object_t::value_type(temp.begin().key(), temp.begin().value()));
        output << tempAnswers;
        cout << tempAnswers << endl;//test
        output.close();
    }

    bool fileConfigVerify() {
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

    void ClearFiles() {
        ofstream clearFile(fileRequests);
        if (clearFile.is_open()) {
            clearFile << (json) {{"requests", {}}};
            clearFile.close();
        }
        clearFile.open(fileAnswers);
        if (clearFile.is_open()) {
            clearFile << (json) {{"answers", {}}};
            clearFile.close();
        }
    }
};