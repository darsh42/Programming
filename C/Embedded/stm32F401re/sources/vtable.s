.syntax unified
.cpu    cortex-m4
.fpu    softvfp
.thumb

.global vtable
.global interrupt_handler

.type vtable, %object
vtable:
    /* handlers 0-15 */
    .word _estack
    .word reset_handler
    .word nmi_handler
    .word hard_fault_handler
    .word memory_management_fault_handler
    .word bus_fault_handler
    .word usage_fault_handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word sv_call_handler
    .word 0
    .word 0
    .word pend_sv_handler
    .word systick_handler
    /* handlers 16-31 */
    .word window_watchdog_irq_handler
    .word pvd_irq_handler
    .word rtc_irq_handler
    .word flash_irq_handler
    .word rcc_irq_handler
    .word exti0_1_irq_handler
    .word exti2_3_irq_handler
    .word exti4_15_irq_handler
    .word 0
    .word dma1_chan_1_irq_handler
    .word dma1_chan_2_3_irq_handler
    .word dma1_chan_4_5_irq_handler
    .word adc1_irq_handler
    .word tim_1_break_irq_handler
    .word tim_1_cc_irq_handler
    .word tim_2_irq_handler
    /* handlers 32-47 */
    .word tim_3_irq_handler
    .word 0
    .word 0
    .word tim_14_irq_handler
    .word 0
    .word tim_16_irq_handler
    .word tim_17_irq_handler
    .word i2c_1_irq_handler
    .word 0
    .word spi_1_irq_handler
    .word 0
    .word usart_1_irq_handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word boot_ram_base
.size vtable, .-vtable
