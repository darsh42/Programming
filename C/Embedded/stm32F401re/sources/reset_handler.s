.syntax unified
.cpu    cortex-m4
.fpu    softvfp
.thumb

/***********************************************************************
*                              Reset Handler                           *
************************************************************************/
.global reset_handler

.type reset_handler, %function
reset_handler:
    /* Set the stack pointer to the end of the stack */
    LDR r0, =_estack
    MOV sp, r0
    
    MOVS r0, #0

    LDR r1, =_sdata
    LDR r2, =_edata
    LDR r3, =_sidata

    B copy_sidata_loop

copy_sidata:
    LDR  r4, [r3, r0]
    STR  r4, [r1, r0]
    ADDS r0, r0, #4

copy_sidata_loop:
    ADDS r4, r0, r1
    CMP  r4, r2
    BCC  copy_sidata

.size reset_handler, .-reset_handler

