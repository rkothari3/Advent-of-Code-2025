/*
 * Pattern -> StartNum - EndNum; separated by commas
 * Invalid Ids:
 * - Sequence of digits repeated twice
 * Sum of invalid IDs is the answer
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool isRepeatedTwice(long long n)
{
    string s = to_string(n);
    int len = s.length();

    if (len % 2 != 0)
        return false;

    int halfLen = len / 2;
    string first = s.substr(0, halfLen);
    string second = s.substr(halfLen);

    return first == second;
}

int main()
{
    ifstream file("input.txt");

    if (!file.is_open())
    {
        cout << "Put fries in the bag vro\n";
        return 1;
    }

    long long startNum, endNum;
    char hyphen, comma;
    long long sum = 0;

    while (file >> startNum >> hyphen >> endNum)
    {
        // optional comma
        file >> comma;

        for (long long j = startNum; j <= endNum; j++)
        {
            if (isRepeatedTwice(j))
            {
                sum += j;
            }
        }
    }

    cout << sum << endl;
    return 0;
}
