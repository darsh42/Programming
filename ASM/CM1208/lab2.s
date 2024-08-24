    # PURPOSE: compare values in registers %eax and %ebx
    #           compare -> restore %eax value


    .section .data

    .section .text
    .globl _start
_start:
    # initialize value of %eax
    movl $10, %eax
    movl $10, %ebx
    movl %eax, %ecx

    subl %ebx, %eax
    jz match_found
    movl $0, %edx
    jmp reset_eax

match_found:
    movl $1, %edx

reset_eax:
    movl %ecx, %eax
    movl %edx, %ebx
    movl $1, %eax
    int $0x80
ret
