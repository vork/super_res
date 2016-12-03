//
// Created by Paul Sanzenbacher on 03.12.16
//

#ifndef SUPER_RES_TIMER_H
#define SUPER_RES_TIMER_H

#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>

#define TIMER_PRECISION 5

/*
 * Timer class for precise measurement of execution times
 */
class Timer
{
public:
    Timer() {
        reset();
    }
    void reset() {
        start = std::chrono::steady_clock::now();
    }
    double elapsed() const {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        return (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0f;
    }
    std::string elapsedString() {
        double e = elapsed();
        std::stringstream stream;
        stream << std::fixed << std::setprecision(TIMER_PRECISION) << e;
        return stream.str();
    }

    void printTimeAndReset(std::string label = "") {
        if (label.size() == 0) {
            label = "elapsed time";
        }
        std::cout << label << ": " << elapsedString() << std::endl;
        reset();
    }

private:
    std::chrono::steady_clock::time_point start;

};

#endif //HALFWAY1_TIMER_H

