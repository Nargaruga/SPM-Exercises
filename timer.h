#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <string>
#include <syncstream>

/*
 *  Class that implements a constructor/destructor-activated timer
 */
class Timer {

  public:
    enum Mode { MILLISECONDS, MICROSECONDS };

    Timer(std::string msg, Mode mode) {
        m_start = std::chrono::system_clock::now();
        m_msg = msg;
        m_mode = mode;
    }

    ~Timer() {
        m_end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = m_end - m_start;
        std::string unit;
        int64_t elapsed;

        switch(m_mode) {
        case Mode::MILLISECONDS: {
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            unit = " msec ";
            break;
        }
        default: {
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
            unit = " usec ";
            break;
        }
        }

        std::cout << m_msg << " computed in " << elapsed << unit << std::endl;
    }

  private:
    std::string m_msg;
    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_end;
    Mode m_mode;
};

#endif