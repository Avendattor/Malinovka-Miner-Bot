#include <iostream>
#include <thread>
#include <chrono>

class Timer {
    bool clear = false;

public:
    template<typename A>
    void setTimeout(A function, int delay);
    template<typename A>
    void setInterval(A function, int interval);
    void stop();

};
template<typename A>
void Timer::setTimeout(A function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if (this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (this->clear) return;
        function();
        });
    t.detach();
}

template<typename A>
void Timer::setInterval(A function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while (true) {
            if (this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if (this->clear) return;
            function();
        }
        });
    t.detach();
}

void Timer::stop() {
    this->clear = true;
}