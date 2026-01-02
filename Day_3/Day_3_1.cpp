#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream file("input.txt");
    // string line = "123453";
    string line;
    string s;
    long long num;
    long long maxNum = 0;
    int sum = 0;
    while(getline(file, line)) {
        // cout << line << endl;
        for (int i = 0; i < line.length() - 1; i++) {
            for (int j = i + 1; j < line.length(); j++) {
                s.push_back(line[i]);
                s.push_back(line[j]);
                num = stoi(s);
                // cout << s << endl;
                s = "";
                if (num > maxNum) {
                    maxNum = num;
                }
            }
        }

        sum += maxNum;
        maxNum = 0;
    }

    cout << sum << endl;
    
    file.close();
    return 0;
}