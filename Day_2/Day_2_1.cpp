/*
* Pattern -> StartNum - EndNum; separated by commas
* Invalid Ids:
* - Sequence of digits
* - No numbers have leading 0
* Sum of invalid IDs is the answer
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ifstream file;
    file.open("input.txt");

    int startNum;
    int endNum;
    char hyphen;

    if (!file.is_open()) {
        cout << "Put fries in the bag vro\n";
        return 1;
    } else {
        if (file >> startNum >> hyphen >> endNum) {
            cout << startNum;
            cout << endNum;
        }
    }
}