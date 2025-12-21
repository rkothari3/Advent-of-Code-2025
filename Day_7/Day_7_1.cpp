#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
using namespace std;

int main()
{
    ifstream file("input.txt");

    vector<string> grid; // Save each row as a element
    string row;

    while (getline(file, row))
    {
        grid.push_back(row);
    }

    // Find starting pos of S
    string startLine = grid[0];
    int S_pos;
    for (int i = 0; i < startLine.length(); i++)
    {
        if (startLine[i] == 'S')
        {
            S_pos = i;
            break;
        }
    }

    set<int> activebeamCols;
    activebeamCols.insert(S_pos);
    int total_splits = 0;

    for (int i = 1; i < grid.size(); i++)
    {
        set<int> beamCols;

        for (int col : activebeamCols)
        {
            if (grid[i][col] == '^')
            {
                total_splits++;
                if (col > 0)
                {
                    beamCols.insert(col - 1);
                }
                if (col < grid[i].length() - 1)
                {
                    beamCols.insert(col + 1);
                }
            }
            else if (grid[i][col] == '.')
            {
                beamCols.insert(col);
            }
        }

        activebeamCols = beamCols;
    }
    cout << total_splits;
    return total_splits;
}