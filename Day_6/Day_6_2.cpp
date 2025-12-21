#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int main() {
    ifstream file("input.txt");

    vector<string> lines;
    string line;

    // Read entire file
    while (getline(file, line)) {
        lines.push_back(line);
    }

    int rows = lines.size();
    int cols = lines[0].size();

    long long grandTotal = 0;
    int col = 0;

    while (col < cols) {

        // Check if column is empty
        bool emptyCol = true;
        for (int r = 0; r < rows; r++) {
            if (lines[r][col] != ' ') {
                emptyCol = false;
                break;
            }
        }

        if (emptyCol) {
            col++;
            continue;
        }

        // Start of a problem
        int startCol = col;

        // Find end of problem
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

        int endCol = col; // [startCol, endCol)

        // Operator is at bottom row
        char op = '?';
        for (int c = startCol; c < endCol; c++) {
            if (lines[rows - 1][c] == '+' || lines[rows - 1][c] == '*') {
                op = lines[rows - 1][c];
                break;
            }
        }

        // Solve this problem (RIGHT → LEFT columns)
        long long result = (op == '+') ? 0 : 1;

        for (int c = endCol - 1; c >= startCol; c--) {

            long long num = 0;
            bool hasDigit = false;

            // Read digits top → bottom
            for (int r = 0; r < rows - 1; r++) {
                char ch = lines[r][c];
                if (ch >= '0' && ch <= '9') {
                    num = num * 10 + (ch - '0');
                    hasDigit = true;
                }
            }

            if (hasDigit) {
                if (op == '+') result += num;
                else result *= num;
            }
        }

        grandTotal += result;
    }

    cout << grandTotal << endl;
    return 0;
}
