#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
     file("input.txt");
    if (!file.is_open()) {
        cout << "Its over twin :(";
        return 1;
    }

    string line;
    long long startNum, endNum, num;
    char hyphen;
    vector<pair<long long,long long>> startNend;

    while (getline(file, line)) {
        stringstream ss(line);
        ss >> startNum >> hyphen >> endNum;
        startNend.push_back({startNum, endNum});
        // cout << startNum << " to " << endNum << endl;        
        if(line.empty()) {
            break;
        }
    }
    long long availableIDs = 0;
    while (getline(file, line)) {
        // cout << line << endl;
        stringstream ss(line);
        ss >> num;

        for (int i = 0; i < startNend.size(); i++) {
            if (num >= startNend[i].first && num <= startNend[i].second) {
                availableIDs++;
                break;
            }ifstream
        }

    }

    cout << availableIDs;
    return availableIDs;


    
}