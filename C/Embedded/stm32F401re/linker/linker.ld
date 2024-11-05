/* Label for programs entry point */
ENTRY(reset_handler)

_sstack = 0x00000400; /* stack size   */
_estack = 0X20018000; /* end of stack */

/************************************************************************************************************************
 *                                                                                                                      *
 *                                              Memory Region definitions                                               *
 *                                                                                                                      *
 ************************************************************************************************************************/
MEMORY
{
    /* Code */
    FLASH  ( rx  ) : ORIGIN = 0X08000000, LENGTH = 0X80000  /* Flash ROM start location and Size                         */
    SYSTEM ( rx  ) : ORIGIN = 0X1FFF0000, LENGTH = 0X07A0F  /* Default Bootloader start location and Size                */

    /* SRAM */
    RAM    ( rxw ) : ORIGIN = 0X20000000, LENGTH = 0X18000  /* RAM start location and Size                               */
    
    /* Peripherals */
    APB1   ( rw  ) : ORIGIN = 0X40000000, LENGTH = 0X07400 /* Advanced Peripheral Bus 1 start location and Size          */
    APB2   ( rw  ) : ORIGIN = 0X40010000, LENGTH = 0X04C00 /* Advanced Peripheral Bus 2 start location and Size          */
    AHB1   ( rw  ) : ORIGIN = 0X40020000, LENGTH = 0X06800 /* Advances High-Performance Bus start location and Size      */
    AHB2   ( rw  ) : ORIGIN = 0X50000000, LENGTH = 0X40000 /* Advances High-Performance Bus start location and Size      */
    CORTEX ( rw  ) : ORIGIN = 0XE0000000, LENGTH = 0X10000 /* ARM Cortex M4 internal Peripherals start location and Size */
}

SECTIONS
{
    /* FLASH Sections*/
    .vector_table :
    {
        . = ALIGN(4);
        KEEP (*(.vector_table))
        . = ALIGN(4);
    } >FLASH
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH
    .rodata :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH
    
    /* RAM Sections */
    _sidata = .;
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } >RAM
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;

    } >RAM
    .dynamic_allocations :
    {
        . = ALIGN(4);
        _ssystem_ram = .;
        . = . + _sstack;
        . = ALIGN(4);
        _esystem_ram = .;
    } >RAM
}
