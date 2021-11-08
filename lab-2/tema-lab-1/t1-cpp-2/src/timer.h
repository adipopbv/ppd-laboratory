#pragma once

#include <ctime>
#include <iostream>

using namespace std;

class Timer {
private:
    clock_t timerStart;

public:
    void start();

    long stop() const;

    void printState() const;

    void printPeriod();
};
