#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

map<string, vector<string>> graph;

int countPaths(const string& current) {
    // Base case: if we reached "out"-> found complete path
    if (current == "out") {
        return 1;
    }
    // Recursive case: try all neighbors; sum up the paths
    else {
        int totalPaths = 0;
        for (const string& neighbor : graph[current]) {
            totalPaths += countPaths(neighbor);
        }
        return totalPaths;
    }
    
}

void parseLine(const string& line) {
    size_t colonPos = line.find(':');  
    string device = line.substr(0, colonPos);    
    string neighbors = line.substr(colonPos + 2); // Skip ": "

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
    
    // Count all paths starting from "you"
    int result = countPaths("you");
    
    cout << result << endl;
    
    return 0;
}