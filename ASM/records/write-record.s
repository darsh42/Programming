    .include "record-def.s"
    .include "linux.s"

    # PURPOSE: Function to write a record to a file
    #          descriptor
    #
    # INPUT: file descriptor
    #
    # OUTPUT: writes data to a file from a buffer and
    #         returns a status code
    #

    # STACK LOCAL VARIABLES
    .equ ST_WRITE_BUFFER, 8
    .equ ST_FILEDES, 12

    .section .text
    .globl write_record
    .type write_record, @function
write_record:
    pushl %ebp
    movl %esp, %ebp

    pushl %ebx
    movl $SYS_WRITE, %eax
    movl ST_FILEDES(%ebp), %ebx
    movl ST_WRITE_BUFFER(%ebp), %ecx
    movl $RECORD_SIZE, %edx
    int $LINUX_SYS_CALL

    # NOTE - %eax has the return value, this will be
    #        passed back to the calling program
    popl %ebx

    movl %ebp, %esp
    popl %ebp
    ret
