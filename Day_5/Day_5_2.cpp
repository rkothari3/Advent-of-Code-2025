#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    ifstream file("input.txt");
    if (!file.is_open())
    {
        cout << "Its over twin :(";
        return 1;
    }

    string line;
    long long startNum, endNum, num;
    char hyphen;
    vector<pair<long long, long long>> startNend;

    while (getline(file, line))
    {
        stringstream ss(line);
        ss >> startNum >> hyphen >> endNum;
        startNend.push_back({startNum, endNum});
        // cout << startNum << " to " << endNum << endl;
        if (line.empty())
        {
            break;
        }
    }
    long long totalFresh;
    sort(startNend.begin(), startNend.end());
    long long curStart = startNend[0].first;
    long long curEnd = startNend[0].second;

    for (int i = 0; i <= startNend.size(); i++)
    {
        long long nextStart = startNend[i].first;
        long long nextEnd = startNend[i].second;

        if (nextStart <= curEnd + 1)
        {
            curEnd = max(curEnd, nextEnd);
        }
        else
        {
            totalFresh += (curEnd - curStart + 1);
            curStart = nextStart;
            curEnd = nextEnd;
        }
    }

    totalFresh += (curEnd - curStart + 1);

    cout << totalFresh;
    return 0;
}
