//
// Created by adipopbv on 10/24/21.
//

#pragma once

#ifndef T1_CPP_TIMER_H
#define T1_CPP_TIMER_H

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

#endif //T1_CPP_TIMER_H
