#include "timer.h"

struct timers timers;

struct timers *get_timers() {
    return &timers;
}

void init_timer() {
    timers.DIV = mem_pointer(mDIV);
    timers.TIMA = mem_pointer(mTIMA);
    timers.TMA = mem_pointer(mTMA);
    timers.TAC = mem_pointer(mTAC);

    timers.m_timer_cycle_inc = CPU_CLOCK/4096;
}

bool enable_timer() {
    return (*timers.TAC & 0b00000100) == 0b00000100;
}

void set_clock_freq() {
    switch(*timers.TAC & 0b00000011) {
        case(0b00): timers.m_timer_cycle_inc = 4096; break;
        case(0b01): timers.m_timer_cycle_inc = 16;   break;
        case(0b10): timers.m_timer_cycle_inc = 64;   break;
        case(0b11): timers.m_timer_cycle_inc = 256;  break;
    }

    return;
}

void update_timers(int cycles) {
    timers.m_divider_cycle_inc -= cycles;
    if (timers.m_divider_cycle_inc <= 0) {
        timers.m_divider_cycle_inc = 255;
        *timers.DIV += cycles;
    }


    if (enable_timer()) {
        timers.m_timer_cycle_inc -= cycles;

        if (timers.m_timer_cycle_inc <= 0) {
            set_clock_freq();

            if (*timers.TIMA == 255) {
                *timers.TIMA = *timers.TMA;
                interrupt_request(0b00000100);
            } else {
                *timers.TIMA += cycles;
            }
        }
    }
}
