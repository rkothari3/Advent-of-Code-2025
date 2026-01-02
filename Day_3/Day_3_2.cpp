#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string maxKDigitsGreedy(const string& s, int k) {
    int n = s.length();
    string result;
    int start = 0;

    for (int pos = 0; pos < k; pos++) {
        char best = '0';
        int bestIndex = start;

        // last index we are allowed to consider
        int end = n - (k - pos);

        for (int i = start; i <= end; i++) {
            if (s[i] > best) {
                best = s[i];
                bestIndex = i;
                if (best == '9') break; // can't do better
            }
        }

        result.push_back(best);
        start = bestIndex + 1;
    }

    return result;
}
int main() {
    ifstream file("input.txt");
    string line;
    long long sum = 0;

    while (getline(file, line)) {
        string best = maxKDigitsGreedy(line, 12);
        sum += stoll(best);
    }

    cout << sum << endl;
    file.close();
    return 0;
}
