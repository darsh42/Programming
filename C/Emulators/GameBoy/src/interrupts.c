#include "interrupts.h"

struct interrupt_handler handler;

struct interrupt_handler *get_handler() {
    return &handler;
}

void init_handler() {
    handler.IF = mem_pointer(mIF);
    handler.IE = mem_pointer(mIE);
}

/* Sets interrupt request for device */
void interrupt_request(uint8_t request) {
    *handler.IF |= request;
}

void service_interrupt(int interrupt) {
    // reset interrupt bit
    *handler.IF = RESET_BIT(*handler.IF, interrupt);

    switch(interrupt) {
        // VBlank service call
        case(0): cpu_ISR(0X40); break;
        // LCD service call
        case(1): cpu_ISR(0X48); break;
        // Timer service call
        case(2): cpu_ISR(0X50); break;
        // Serial service call
        case(3): cpu_ISR(0X58); break;
        // Joypad service call
        case(4): cpu_ISR(0X60); break;
    }
}

void handle_interrupts() {
    // If no master interrupt
    if (!cpu_IME()) return;

    // if no interrupt requested
    if (*handler.IF == 0) return;

    // if no interrupt is enabled
    if (*handler.IE == 0) return;

    // test for interrupts in prority order
    for (int i = 0; i < 5; i++) {
        if (TEST_BIT(*handler.IF, i) && TEST_BIT(*handler.IE, i)) {
            service_interrupt(i);
        }
    }
    return;
}
