#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "Put fries in bag vro\n";
        return 1;
    }

    int dial = 50;
    int count_zero = 0;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        char direction = line[0];
        int distance = stoi(line.substr(1));

        if (direction == 'L')
            dial = (dial - distance) % 100;
        else
            dial = (dial + distance) % 100;

        if (dial < 0) dial += 100;

        if (dial == 0)
            count_zero++;
    }

    cout << count_zero << endl;
    return 0;
}
