#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isRepeated(long long n)
{
    string s = to_string(n);
    int len = s.length();

    for (int patternLen = 1; patternLen <= len / 2; patternLen++)
    {
        if (len % patternLen != 0)
            continue;

        string pattern = s.substr(0, patternLen);
        string built = "";

        int repeatCount = len / patternLen;
        for (int i = 0; i < repeatCount; i++)
            built += pattern;

        if (built == s)
            return true;
    }

    return false;
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
        file >> comma; // optional comma

        for (long long j = startNum; j <= endNum; j++)
        {
            if (isRepeated(j))
                sum += j;
        }
    }

    cout << sum << endl;
    return 0;
}
