#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main() {
    ifstream inputFile("input.txt");
    
    if (!inputFile.is_open()) {
        cout << "Error: Could not open input.txt" << endl;
        return 1;
    }
    
    string line;
    int totalPresses = 0;
    
    while (getline(inputFile, line)) {
        if (line.empty()) continue;
        
        int targetState = 0;
        int numLights = 0;
        size_t start = line.find('[') + 1;
        size_t end = line.find(']');
        string targetStr = line.substr(start, end - start);
        numLights = targetStr.length();
        for (int i = 0; i < numLights; i++) {
            if (targetStr[i] == '#') {
                targetState |= (1 << i);
            }
        }
        
        vector<int> buttons;
        size_t pos = end + 1;
        size_t bracePos = line.find('{');
        
        while (pos < bracePos) {
            size_t parenStart = line.find('(', pos);
            if (parenStart == string::npos || parenStart >= bracePos) break;
            
            size_t parenEnd = line.find(')', parenStart);
            string buttonStr = line.substr(parenStart + 1, parenEnd - parenStart - 1);
            
            int buttonMask = 0;
            stringstream ss(buttonStr);
            string num;
            while (getline(ss, num, ',')) {
                int lightIndex = stoi(num);
                buttonMask |= (1 << lightIndex);
            }
            buttons.push_back(buttonMask);
            
            pos = parenEnd + 1;
        }
        
        int numButtons = buttons.size();
        int minPresses = numButtons + 1;
        
        for (int mask = 0; mask < (1 << numButtons); mask++) {
            int currentState = 0;
            int presses = 0;
            
            for (int b = 0; b < numButtons; b++) {
                if (mask & (1 << b)) {
                    currentState ^= buttons[b];
                    presses++;
                }
            }
            
            if (currentState == targetState) {
                if (presses < minPresses) {
                    minPresses = presses;
                }
            }
        }
        
        totalPresses += minPresses;
    }
    
    inputFile.close();
    
    cout << totalPresses << endl;
    
    return 0;
}
