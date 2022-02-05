#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include <map>
#include <iomanip>

using namespace std;


class PageRank {
private:
    int value = 1;
    map<string, int> mapper;
    map<string, vector<string>> graph;
    map<string, vector<pair<string, double>>> list;
public:
    void setAdjList();

    void insertEdge(const string &from, const string &to);

    vector<pair<string, double>> getAdjacent(const string &vertex);

    int getOutdegree(const string &vertex);

    double getScore(map<string, double> &score, string &vertex);

    void initialScore(map<string, double> &score);

    void updateRow(map<string, double> &score);

    void setNonOutdegree(map<string, double> &score);

    void print(map<string, double> &score);

    void getUpdate(map<string, double> &score, int power);
};

void PageRank::insertEdge(const string &from, const string &to) {
    if (mapper.find(from) == mapper.end()) {
        mapper[from] = value++;
    }
    if (mapper.find(to) == mapper.end()) {
        mapper[to] = value++;
    }
    graph[from].push_back(to);
}

void PageRank::setAdjList() {
    for (const auto &it: graph) {
        for (int i = 0; i < it.second.size(); i++) {
            list[it.first].push_back(make_pair(it.second[i], (double) 1 / (double) it.second.size()));
        }
    }
}

vector<pair<string, double>> PageRank::getAdjacent(const string &vertex) {
    vector<pair<string, double>> val;
    for (auto &iter: list) {
        for (auto it = iter.second.begin(); it != iter.second.end(); it++) {
            if (vertex == it->first) {
                val.emplace_back(iter.first, it->second);
            }
        }
    }
    return val;
}

int PageRank::getOutdegree(const string &vertex) {
    int count = 0;
    for (auto &it: graph) {
        if (it.first == vertex) {
            for (int i = 0; i < it.second.size(); i++) {
                count++;
            }
        }
    }
    return count;
}

double PageRank::getScore(map<string, double> &score, string &vertex) {
    double rowVal;
    int count = 1;
    vector<pair<string, double>> val = getAdjacent(vertex);
    for (auto &it: val) {
        if (count == 1) {
            rowVal = it.second * score[it.first];
            count++;
        } else {
            rowVal += it.second * score[it.first];
        }
    }
    return rowVal;
}

void PageRank::initialScore(map<string, double> &score) {
    for (auto &itr: graph) {
        score[itr.first] = (double) (1) / (double) (mapper.size());
    }
}

void PageRank::updateRow(map<string, double> &score) {
    vector<double> temp;
    for (auto &iter: graph) {
        string web = iter.first;
        temp.push_back(getScore(score, web));
    }
    auto it = score.begin();
    for (double i: temp) {
        score[it->first] = i;
        it++;
    }
}

void PageRank::setNonOutdegree(map<string, double> &score) {
    for (auto &iter: mapper) {
        if (getOutdegree(iter.first) == 0) {
            score[iter.first] = double(1) / double(mapper.size());
        }
    }
}

void PageRank::print(map<string, double> &score) {
    for (auto &itr: score) {
        cout << itr.first << " " << fixed << setprecision(2) << itr.second << endl;
    }
}

void PageRank::getUpdate(map<string, double> &score, int power) {
    for (int i = 0; i < power - 1; i++) {
        updateRow(score);
    }
}

int main() {
    PageRank pr;
    map<string, double> score;
    int no_of_lines, power_iterations;
    string from, to;
    cin >> no_of_lines;
    cin >> power_iterations;
    for (int i = 0; i < no_of_lines; ++i) {
        cin >> from;
        cin >> to;
        pr.insertEdge(from, to);
    }

    pr.setAdjList();
    pr.initialScore(score);
    pr.getUpdate(score, power_iterations);
    pr.setNonOutdegree(score);
    pr.print(score);

    return 0;
}
