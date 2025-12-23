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

    int connectionsMade = 0;
    int lastIdx1, lastIdx2;

    for (int i = 0; i < distArr.size(); i++)
    {
        int idx1 = get<1>(distArr[i]);
        int idx2 = get<2>(distArr[i]);
        
        // Only unite if they're in different circuits
        if (find(idx1, parent) != find(idx2, parent))
        {
            unite(idx1, idx2, parent);
            connectionsMade++;
            lastIdx1 = idx1;
            lastIdx2 = idx2;
            
            // Stop after n-1 connections (all in one circuit)
            if (connectionsMade == arr.size() - 1)
            {
                break;
            }
        }
    }

    long long result = (long long)get<0>(arr[lastIdx1]) * get<0>(arr[lastIdx2]);
    cout << result << endl;

    return 0;
}
