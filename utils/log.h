//
// Some logging utilities
// 1. "log() << ..." will show a time stamp first
// 2. "print(a, b, c)" is python-like print for any a, b, c that has operator<< overloaded. For example,
//      int a = 10;
//      double b = 3.14;
//      std::string c = "Gengjie";
//      print(a, b, c);
//     This code piece will show "10 3.14 Gengjie".
//

#pragma once

#include <chrono>
#include <iostream>
#include <string>

class timer {
    using clock = std::chrono::steady_clock;

private:
    clock::time_point _start;

public:
    timer();
    void start();
    double elapsed() const;  // seconds
};

std::ostream& operator<<(std::ostream& os, const timer& t);
std::string to_string(const timer& t);

std::ostream& log(std::ostream& os = std::cout);

// print(a, b, c)
inline void print() { std::cout << std::endl; }
template <typename T, typename... TAIL>
void print(const T& t, TAIL... tail) {
    std::cout << t << ' ';
    print(tail...);
}