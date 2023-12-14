#ifndef INTERRUPTS_H_INCLUDED
#define INTERRUPTS_H_INCLUDED

#include "common.h"

#define mIF 0XFF0F
#define mIE 0XFFFF

struct interrupt_handler {

    /*
    ** Interrupt Flag - ADDR: 0XFF0F
    ** - VBlank (READ/WRITE) - bit 0 - Controls whether VBlank handler is requested
    ** - LCD    (READ/WRITE) - bit 1 - Controls whether LCD handler is requested
    ** - Timer  (READ/WRITE) - bit 2 - Controls whether Timer handler is requested
    ** - Serial (READ/WRITE) - bit 3 - Controls whether Serial handler is requested
    ** - Joypad (READ/WRITE) - bit 4 - Controls whether Joypad handler is requested
    */
    uint8_t *IF;
    /*
    ** Interrupt ENABLE - ADDR: 0XFFFF
    ** - VBlank (READ/WRITE) - bit 0 - Controls whether VBlank handler can be called
    ** - LCD    (READ/WRITE) - bit 1 - Controls whether LCD handler can be called
    ** - Timer  (READ/WRITE) - bit 2 - Controls whether Timer handler can be called
    ** - Serial (READ/WRITE) - bit 3 - Controls whether Serial handler can be called
    ** - Joypad (READ/WRITE) - bit 4 - Controls whether Joypad handler can be called
    */
    uint8_t *IE;
};

/*
** Interrupt device connnections
** cpu_IME -> IME enabled?
**
*/
extern void cpu_ISR_start(uint8_t service_routine_addr);
extern void cpu_ISR_return();
extern bool cpu_IME();

/* Memory functions */
extern uint8_t *mem_pointer(uint16_t addr);

#endif // INTERRUPTS_H_INCLUDED
