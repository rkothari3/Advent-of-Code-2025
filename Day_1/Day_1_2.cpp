#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Put fries in bag vro\n";
        return 1;
    }

    long long dial = 50;
    long long count_zero = 0;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        char direction = line[0];
        long long distance = stoll(line.substr(1));

        long long k;
        if (direction == 'R') {
            k = (100 - (dial % 100)) % 100;
        } else {
            k = (dial % 100);
        }
        if (k == 0) k = 100;

        if (distance >= k) {
            count_zero += 1 + (distance - k) / 100;
        }

        if (direction == 'R')
            dial = (dial + distance) % 100;
        else
            dial = (dial - distance) % 100;

        if (dial < 0) dial += 100;
    }

    cout << count_zero << '\n';
    return 0;
}
