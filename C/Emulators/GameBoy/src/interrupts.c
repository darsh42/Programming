#include "interrupts.h"

struct interrupt_handler handler;

void init_handler() {
    handler.IF = mem_pointer(mIF);
    handler.IE = mem_pointer(mIE);
}

/* Sets interrupt request for device */
void interrupt_request(uint8_t request) {
    *handler.IF |= request;
}

void handle_interrupts() {
    // If no master interrupt
    if (!cpu_IME()) return;

    // if no interrupt requested
    if (!(*handler.IF)) return;

    // if no interrupt is enabled
    if (!(*handler.IE)) return;

    uint8_t service = *handler.IE & *handler.IF;

    // VBlank service call
    if (TEST_BIT(service, 0)) {
       cpu_ISR_start(0X40);
       cpu_ISR_return();
    }

    // LCD service call
    if (TEST_BIT(service, 1)) {
       cpu_ISR_start(0X48);
       cpu_ISR_return();
    }

    // Timer service call
    if (TEST_BIT(service, 2)) {
       cpu_ISR_start(0X50);
       cpu_ISR_return();
    }

    // Serial service call
    if (TEST_BIT(service, 3)) {
       cpu_ISR_start(0X58);
       cpu_ISR_return();
    }

    // Joypad service call
    if (TEST_BIT(service, 4)) {
       cpu_ISR_start(0X60);
       cpu_ISR_return();
    }

    return;
}
