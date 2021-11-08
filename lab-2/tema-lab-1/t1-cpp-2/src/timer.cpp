#include "timer.h"

void Timer::start() {
    timerStart = clock();
}

long Timer::stop() const {
    float seconds = ((float(clock() - timerStart)) / CLOCKS_PER_SEC);
    long milliseconds = long(seconds * 1000);
    return milliseconds;
}

void Timer::printState() const {
    cout << long(((float(clock() - timerStart)) / CLOCKS_PER_SEC) * 1000);
}

void Timer::printPeriod() {
    clock_t stop = clock();
    cout << long(((float(stop - timerStart)) / CLOCKS_PER_SEC) * 1000);
    timerStart = stop;
}
