#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

int main() {
    ifstream file("input.txt");
    vector<pair<int, int>> redTiles;
    string line;
    
    while(getline(file, line)) {
        int x, y;
        sscanf(line.c_str(), "%d,%d", &x, &y);
        redTiles.push_back({x, y});
    }
    file.close();

    int n = redTiles.size();
    
    vector<int> xs, ys;
    xs.reserve(n);
    ys.reserve(n);
    
    for (auto& tile : redTiles) {
        xs.push_back(tile.first);
        ys.push_back(tile.second);
    }
    
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    
    int W = xs.size(), H = ys.size();
    
    auto getXIndex = [&](int x) { return lower_bound(xs.begin(), xs.end(), x) - xs.begin(); };
    auto getYIndex = [&](int y) { return lower_bound(ys.begin(), ys.end(), y) - ys.begin(); };
    
    vector<bool> horEdge(H * W, false);
    vector<bool> verEdge(W * H, false);
    
    auto horIndex = [&](int row, int col) { return row * W + col; };
    auto verIndex = [&](int col, int row) { return col * H + row; };
    
    for (int k = 0; k < n; k++) {
        int x1 = redTiles[k].first, y1 = redTiles[k].second;
        int x2 = redTiles[(k+1) % n].first, y2 = redTiles[(k+1) % n].second;
        
        int i1 = getXIndex(x1), j1 = getYIndex(y1);
        int i2 = getXIndex(x2), j2 = getYIndex(y2);
        
        if (i1 == i2) {
            if (j1 > j2) swap(j1, j2);
            for (int j = j1; j < j2; j++) {
                verEdge[verIndex(i1, j)] = true;
            }
        } else {
            if (i1 > i2) swap(i1, i2);
            for (int i = i1; i < i2; i++) {
                horEdge[horIndex(j1, i)] = true;
            }
        }
    }
    
    int cellW = W - 1, cellH = H - 1;
    vector<bool> outside(cellH * cellW, false);
    queue<pair<int, int>> q;
    
    auto cellIndex = [&](int row, int col) { return row * cellW + col; };
    
    for (int i = 0; i < cellW; i++) {
        if (!horEdge[horIndex(0, i)]) {
            outside[cellIndex(0, i)] = true;
            q.push({0, i});
        }
        if (!horEdge[horIndex(H-1, i)]) {
            outside[cellIndex(cellH-1, i)] = true;
            q.push({cellH-1, i});
        }
    }
    
    for (int j = 0; j < cellH; j++) {
        if (!verEdge[verIndex(0, j)]) {
            outside[cellIndex(j, 0)] = true;
            q.push({j, 0});
        }
        if (!verEdge[verIndex(W-1, j)]) {
            outside[cellIndex(j, cellW-1)] = true;
            q.push({j, cellW-1});
        }
    }
    
    while (!q.empty()) {
        auto [row, col] = q.front();
        q.pop();
        
        if (row > 0 && !outside[cellIndex(row-1, col)] && !horEdge[horIndex(row, col)]) {
            outside[cellIndex(row-1, col)] = true;
            q.push({row-1, col});
        }
        if (row < cellH-1 && !outside[cellIndex(row+1, col)] && !horEdge[horIndex(row+1, col)]) {
            outside[cellIndex(row+1, col)] = true;
            q.push({row+1, col});
        }
        if (col > 0 && !outside[cellIndex(row, col-1)] && !verEdge[verIndex(col, row)]) {
            outside[cellIndex(row, col-1)] = true;
            q.push({row, col-1});
        }
        if (col < cellW-1 && !outside[cellIndex(row, col+1)] && !verEdge[verIndex(col+1, row)]) {
            outside[cellIndex(row, col+1)] = true;
            q.push({row, col+1});
        }
    }
    
    vector<int> prefix((cellH+1) * (cellW+1), 0);
    auto prefIndex = [&](int row, int col) { return row * (cellW+1) + col; };
    
    for (int row = 0; row < cellH; row++) {
        for (int col = 0; col < cellW; col++) {
            int inside = outside[cellIndex(row, col)] ? 0 : 1;
            prefix[prefIndex(row+1, col+1)] = prefix[prefIndex(row, col+1)] + 
                                               prefix[prefIndex(row+1, col)] - 
                                               prefix[prefIndex(row, col)] + inside;
        }
    }
    
    vector<pair<int, int>> comp(n);
    for (int i = 0; i < n; i++) {
        comp[i] = {getXIndex(redTiles[i].first), getYIndex(redTiles[i].second)};
    }
    
    long long maxArea = 0;
    
    for (int a = 0; a < n; a++) {
        int i1 = comp[a].first, j1 = comp[a].second;
        
        for (int b = a+1; b < n; b++) {
            int i2 = comp[b].first, j2 = comp[b].second;
            
            int left = min(i1, i2), right = max(i1, i2);
            int bottom = min(j1, j2), top = max(j1, j2);
            
            if (left == right || bottom == top) continue;
            
            int totalCells = (right - left) * (top - bottom);
            int insideCells = prefix[prefIndex(top, right)] - 
                             prefix[prefIndex(bottom, right)] - 
                             prefix[prefIndex(top, left)] + 
                             prefix[prefIndex(bottom, left)];
            
            if (insideCells == totalCells) {
                long long width = (long long)xs[right] - xs[left] + 1;
                long long height = (long long)ys[top] - ys[bottom] + 1;
                long long area = width * height;
                maxArea = max(maxArea, area);
            }
        }
    }
    
    cout << "Max Area: " << maxArea << endl;
    return 0;
}
