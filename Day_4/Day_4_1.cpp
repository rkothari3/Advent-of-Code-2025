#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream file("input.txt");

    vector<string> grid;
    string line;
    while (file >> line)
    {
        grid.push_back(line);
    }

    int rows = grid.size();
    int cols = grid[0].size();

    int count = 0;

    // neighbors
    int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] != '@')
                continue;

            int neighbors = 0;

            for (int i = 0; i < 8; i++)
            {
                int nr = r + dr[i];
                int nc = c + dc[i];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols)
                {
                    if (grid[nr][nc] == '@')
                    {
                        neighbors++;
                    }
                }
            }

            if (neighbors < 4)
            {
                count++;
            }
        }
    }

    cout << count << endl;
    return 0;
}