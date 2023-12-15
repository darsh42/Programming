#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "common.h"


struct timers {
    /*
    ** Divider Register - ADDR: 0XFF04
    ** - Incremented at 16384Hz
    ** - Always incremented
    ** - Any write sets value to 0X00
    */
    uint8_t *DIV;

    /*
    ** Timer Counter -  ADDR: 0XFF05
    ** - Timer incremented, speed controlled by TAC
    ** - When overflow, reset to TMA and interrupt
    */
    uint8_t *TIMA;

    /*
    ** Timer Modulo - ADDR: 0XFF06
    ** - Contains reset value of TIMA
    ** - if at 0XFF, interrupt request at clock frequency
    **   if at 0XFE, interrupt request at half the clock frequency
    **   if at 0XFD, interrupt request at third the clock frequency, etc.
    */
    uint8_t *TMA;

    /*
    ** Timer Control - ADDR: 0XFF07
    ** - Enable: bit 2 -> Controls when TIMA is incremented
    ** - Clock select: bits 0-1 -> Controls speed of clock incrementation
    */
    uint8_t *TAC;

    /*
    ** Will dec until 0, then inc TIMA
    */
    int m_timer_cycle_inc;
    /*
    ** Will dec until 0, then inc DIV
    */
    int m_divider_cycle_inc;
};

/* Timer Interrupts */
extern void interrupt_request(uint8_t request);

/* Memory functions */
extern uint8_t *mem_pointer(uint16_t addr);

#endif // TIMER_H_INCLUDED
