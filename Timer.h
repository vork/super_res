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
        markers.clear();
    }

    // get elapsed time in milliseconds
    double elapsed() const {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        return (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0f;
    }

    // formated elampsed time
    std::string elapsedString() {
        std::stringstream stream;
        stream << timeToString(elapsed());
        return stream.str();
    }

    // print time with label (and markers if used) and reset timer
    void printTimeAndReset(std::string label = "") {
        if (label.size() == 0) {
            label = "elapsed time";
        }
        std::cout << label << ": " << elapsedString();

        if (markers.size() > 0) {
            std::cout << " (markers: " << markers.size()  << "; average: ";
            double avg = (elapsed() / markers.size());
            std::cout << avg << ")";
        }

        std::cout << std::endl;
        reset();
    }

    // add a new marker (can be used when analyzing iterations)
    void setMarker() {
        markers.push_back(std::chrono::steady_clock::now());
    }

private:
    std::chrono::steady_clock::time_point start;
    std::vector<std::chrono::steady_clock::time_point> markers;

    std::string timeToString(double time) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(TIMER_PRECISION) << time;
        return stream.str();
    }

};

#endif //HALFWAY1_TIMER_H

