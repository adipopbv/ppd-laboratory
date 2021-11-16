#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "timer.h"

using namespace std;

void stoia(const string &stringValue, int arrayValue[], int arraySize) {
    for (int i = 0; i < stringValue.size(); ++i) {
        arrayValue[i] = stringValue[i] - '0';
    }
    for (int i = (int) stringValue.size(); i < arraySize; ++i) {
        arrayValue[i] = 0;
    }
}

int main() {
    string line;
    string firstNumberStr, secondNumberStr;
    ifstream fin("../Numar1.txt");
    getline(fin, line);
    int firstNumberSize = stoi(line);
    getline(fin, firstNumberStr);
    fin.close();
    fin = ifstream("../Numar2.txt");
    getline(fin, line);
    int secondNumberSize = stoi(line);
    getline(fin, secondNumberStr);
    fin.close();

    if (firstNumberSize < secondNumberSize) {
        swap(firstNumberStr, secondNumberStr);
        swap(firstNumberSize, secondNumberSize);
    }
    int greatestNumberSize = firstNumberSize;
    reverse(firstNumberStr.begin(), firstNumberStr.end());
    reverse(secondNumberStr.begin(), secondNumberStr.end());

    int firstNumber[greatestNumberSize], secondNumber[greatestNumberSize], result[greatestNumberSize + 1];
    stoia(firstNumberStr, firstNumber, greatestNumberSize);
    stoia(secondNumberStr, secondNumber, greatestNumberSize);

    Timer timer = Timer();
    timer.start();
    int carry = 0;
    for (int i = 0; i < greatestNumberSize; ++i) {
        int currentSum = (firstNumber[i] + secondNumber[i] + carry);
        carry = currentSum / 10;
        result[i] = currentSum % 10;
    }
    result[greatestNumberSize] = carry;
    cout << timer.stop();

    /*
    for (int i = greatestNumberSize; i >= 0; --i) {
        if (i == greatestNumberSize) {
            if (result[greatestNumberSize] != 0)
                cout << result[greatestNumberSize];
        } else
            cout << result[i];
    }
     */
    return 0;
}
