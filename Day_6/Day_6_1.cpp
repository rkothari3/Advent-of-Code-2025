#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

string trim(string s) {
    while (!s.empty() && s[0] == ' ') s.erase(0, 1);
    while (!s.empty() && s.back() == ' ') s.pop_back();
    return s;
}

int main() {
    ifstream file("input.txt");

    vector<string> lines;
    string line;

    // 1. Read entire file line by line
    while (getline(file, line)) {
        lines.push_back(line);
    }

    int rows = lines.size();
    int cols = lines[0].size();

    long long grandTotal = 0;

    // 2. Scan column by column
    int col = 0;
    while (col < cols) {

        // Check if this column is empty
        bool emptyCol = true;
        for (int r = 0; r < rows; r++) {
            if (lines[r][col] != ' ') {
                emptyCol = false;
                break;
            }
        }

        // Skip empty columns
        if (emptyCol) {
            col++;
            continue;
        }

        // 3. Found start of a problem
        int startCol = col;

        // Move until we hit an empty column
        while (col < cols) {
            bool allSpaces = true;
            for (int r = 0; r < rows; r++) {
                if (lines[r][col] != ' ') {
                    allSpaces = false;
                    break;
                }
            }
            if (allSpaces) break;
            col++;
        }

        int endCol = col; // problem is [startCol, endCol)

        // 4. Read numbers and operator
        vector<long long> nums;
        char op = '?';

        for (int r = 0; r < rows; r++) {
            string part = lines[r].substr(startCol, endCol - startCol);
            part = trim(part);

            if (part == "+" || part == "*") {
                op = part[0];
            }
            else if (!part.empty()) {
                nums.push_back(stoll(part));
            }
        }

        // 5. Solve this problem
        long long result;
        if (op == '+') result = 0;
        else result = 1;

        for (long long x : nums) {
            if (op == '+') result += x;
            else result *= x;
        }

        grandTotal += result;
    }

    cout << grandTotal << endl;
    return 0;
}
