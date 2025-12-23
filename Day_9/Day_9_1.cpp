#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ifstream file("input.txt");
    vector<pair<int, int>> indices;
    string line;
    char comma;
    long long row, col;

    while(getline(file, line)) {
        stringstream ss(line);
        ss >> row >> comma >> col;
        cout << "x: " << row << " " << "y: " << col << endl;
        indices.push_back({row, col});
    }
    long maxArea = 0;
    for (int i = 0; i < indices.size(); i++) {
        for (int j = 0; j < indices.size(); j++) {
            long length = abs(indices[i].first - indices[j].first) + 1;
            long width = abs(indices[i].second - indices[j].second) + 1;
            long area = length * width;
            if (area > maxArea) {
                maxArea = area;
            }
        }
    }

    cout << "Max Area: " << maxArea << endl;
    return 0;
    
}