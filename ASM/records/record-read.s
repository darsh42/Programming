    .include "record-def.s"
    .include "linux.s"

    # PURPOSE: Function to read a record from a file
    #          descriptor
    #
    # INPUT: file descriptor
    #
    # OUTPUT: writes data to a buffer and returns
    #         a status code
    #

    # STACK LOCAL VARIABLES
    .equ ST_READ_BUFFER, 8
    .equ ST_FILEDES, 12

    .section .text
    .globl read_record
    .type read_record, @function
read_record:
    pushl %ebp
    movl %esp, %ebp

    pushl %ebx
    movl $SYS_READ, %eax
    movl ST_FILEDES(%ebp), %ebx
    movl ST_READ_BUFFER(%ebp), %ecx
    movl $RECORD_SIZE, %edx
    int  $LINUX_SYS_CALL

    # NOTE - %eax has the return value, this will be
    #        passed back to the calling program

    popl %ebx

    movl %ebp, %esp
    popl %ebp
    ret
