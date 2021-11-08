#pragma once

#include <mutex>
#include <condition_variable>

class barrier {
public:
    explicit barrier(int count) : counter(0), waiting(0), thread_count(count) {}

    void wait() {
        //fence mechanism
        std::unique_lock<std::mutex> lk(m);
        ++counter;
        ++waiting;
        cv.wait(lk, [&] { return counter >= thread_count; });
        cv.notify_one();
        --waiting;
        if (waiting == 0) {  //reset barrier
            counter = 0;
        }
        lk.unlock();
    }

private:
    std::mutex m{};
    std::condition_variable cv{};
    int counter;
    int waiting;
    int thread_count;
};
