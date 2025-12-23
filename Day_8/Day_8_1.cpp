#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <cmath>
using namespace std;

int find(int x, vector<int> &parent)
{
    if (parent[x] != x)
    {
        parent[x] = find(parent[x], parent); // path compression
    }
    return parent[x];
}

void unite(int x, int y, vector<int> &parent)
{
    int rootX = find(x, parent);
    int rootY = find(y, parent);
    if (rootX != rootY)
    {
        parent[rootX] = rootY;
    }
}

int main()
{
    ifstream file("input.txt");
    string line;
    long long X, Y, Z;
    char comma;

    vector<tuple<int, int, int>> arr;     // x, y, z
    vector<tuple<double, int, int>> distArr; // distance, index_i, index_j

    while (getline(file, line))
    {
        stringstream ss(line);
        ss >> X >> comma >> Y >> comma >> Z;
        arr.emplace_back(X, Y, Z);
    }

    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = i + 1; j < arr.size(); j++)
        {
            double dist = sqrt(pow(get<0>(arr[i]) - get<0>(arr[j]), 2) +
                           pow(get<1>(arr[i]) - get<1>(arr[j]), 2) +
                           pow(get<2>(arr[i]) - get<2>(arr[j]), 2));
            distArr.emplace_back(dist, i, j);
        }
    }

    sort(distArr.begin(), distArr.end());

    // Union-Find
    vector<int> parent(arr.size());
    for (int i = 0; i < parent.size(); i++)
    {
        parent[i] = i;
    }

    for (int i = 0; i < 1000; i++)
    {
        int idx1 = get<1>(distArr[i]);
        int idx2 = get<2>(distArr[i]);
        unite(idx1, idx2, parent);
    }

    // Count circuit sizes
    map<int, int> circuitSize;
    for (int i = 0; i < arr.size(); i++)
    {
        circuitSize[find(i, parent)]++;
    }

    // Extract sizes and sort in descending order
    vector<int> sizes;
    for (auto &p : circuitSize)
    {
        sizes.push_back(p.second);
    }
    sort(sizes.rbegin(), sizes.rend());

    // Multiply the top 3
    long long result = (long long)sizes[0] * sizes[1] * sizes[2];
    cout << result << endl;

    return 0;
}
