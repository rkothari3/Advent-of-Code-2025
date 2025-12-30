#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <tuple> // for dp
using namespace std;

map<string, vector<string>> graph;
map<tuple<string, bool, bool>, long long> memo;

long long countPaths(const string& current, bool visitedDac, bool visitedFft) {
    if (current == "dac") visitedDac = true;
    if (current == "fft") visitedFft = true;
    
    if (current == "out") {
        return (visitedDac && visitedFft) ? 1 : 0;
    }
    
    auto key = make_tuple(current, visitedDac, visitedFft);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }
    
    long long totalPaths = 0;
    for (const string& neighbor : graph[current]) {
        totalPaths += countPaths(neighbor, visitedDac, visitedFft);
    }
    
    memo[key] = totalPaths;
    return totalPaths;
}

void parseLine(const string& line) {
    size_t colonPos = line.find(':');  
    string device = line.substr(0, colonPos);    
    string neighbors = line.substr(colonPos + 2);

    istringstream iss(neighbors);
    string neighbor;

    while (iss >> neighbor) {
        graph[device].push_back(neighbor);
    }
}

int main() {
    ifstream file("input.txt");
    
    if (!file.is_open()) {
        cerr << "cooked" << endl;
        return 1;
    }
    
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            parseLine(line);
        }
    }
    
    file.close();
    
    long long result = countPaths("svr", false, false);
    
    cout << result << endl;
    
    return 0;
}
