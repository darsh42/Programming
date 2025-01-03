#PURPOSE: conver document to uppercase
#
#
#
#
#PROCESSING: 1) open input file
#            2) open the output file
#            4) While we're not at the end of input file
#               a) read file into buffer
#               b) go through each byte of memory
#                    if the byte is lowercase letter,
#                    convert it to upper-case
#               c) write the memory buffer to output
#

    .section .data
    # ####### CONSTANTS ######## ;;
    .equ SYS_OPEN, 5
    .equ SYS_WRITE, 4
    .equ SYS_READ, 3
    .equ SYS_CLOSE, 6
    .equ SYS_EXIT, 1

    # options for open
    # /usr/include/asm/fcntl.h
    .equ O_RDONLY, 0
    .equ O_CREAT_WRONLY_TRUNC, 03101

    # standard file descriptors
    .equ STDIN, 0
    .equ STDOUT, 1
    .equ STDERR, 2

    # system call interrupt
    .equ LINUX_SYSCALL, 0x80
    .equ END_OF_FILE, 0
    .equ NUMBER_ARGUMENTS, 2

    # system error messages
    error_msg:
        .ascii "ERROR\0"
    error_msg_len = .-error_msg

    .section .bss
    # Buffer - location where data is loaded from file
    #          into buffer, this should not be greater than
    #          16,000 for reasons
    .equ FD_SIZE, 1
    .equ BUFFER_SIZE, 500
    .lcomm BUFFER_DATA, BUFFER_SIZE
    .lcomm BF_FD_IN, FD_SIZE
    .lcomm BF_FD_OUT, FD_SIZE

    .section .text

    # STACK POSITIONS
    .equ ST_SIZE_RESERVE, 8
    .equ ST_FD_IN, -4
    .equ ST_FD_OUT, -8
    .equ ST_ARGC, 0             # numebr of arguments
    .equ ST_ARGV_0, 4           # name of program
    .equ ST_ARGV_1, 8           # input filename
    .equ ST_ARGV_2, 12          # output filename

    .globl _start
_start:
    # START PROGRAM ;;
    # SAVE stack pointer ;;
    movl %esp, %ebp

    # Allocate space for file descriptors on stack
    subl $ST_SIZE_RESERVE, %esp

    # check for argument number
    # no argument goto STD in and out
    cmpl $1, ST_ARGC(%ebp)
    je STD_files

    # if 2 arguments are found go ro read files
    cmpl $3, ST_ARGC(%ebp)
    je open_files

    jmp error


open_files:
open_fd_in:
    # OPEN INPUT FILE ;;
    # sys call in %eax
    movl $SYS_OPEN, %eax

    #  input filename
    movl ST_ARGV_1(%ebp), %ebx

    # read only flag
    movl $O_RDONLY, %ecx

    # move permission
    movl $0666, %edx

    # SYS call Linux
    int $LINUX_SYSCALL

handle_fd_in:
    # if the file descriptor
    # is negetive
    cmpl $0, %eax
    jle error

    # save file descriptor
    movl %eax, ST_FD_IN(%ebp)

open_fd_out:
    #  OPEN OUTPUT FILE ;;
    movl $SYS_OPEN, %eax

    movl ST_ARGV_2(%ebp), %ebx

    movl $O_CREAT_WRONLY_TRUNC, %ecx

    movl $0666, %edx

    int $LINUX_SYSCALL

handle_fd_out:
    # if the file descriptor
    # is negetive
    cmpl $0, %eax
    jle error

    movl %eax, ST_FD_OUT(%ebp)
    jmp read_loop_begin

STD_files:
STD_IN:
    # Save STD in FD
    movl $STDIN, BF_FD_IN
STD_OUT:
    # Save STD out FD
    movl $STDOUT, BF_FD_OUT


read_loop_begin:

    #  READ BLOCK FROM SYSTEM
    movl $SYS_READ, %eax

    # get the input file descriptor
    movl ST_FD_IN(%ebp), %ebx

    # specify the buffer
    movl $BUFFER_DATA, %ecx

    # specify the size of buffer
    movl $BUFFER_SIZE, %edx

    # SYSCALL
    int $LINUX_SYSCALL

    # EXIT if end is reached
    cmpl $END_OF_FILE, %eax
    jle end_loop


continue_read_loop:
    # CONVERT BLOCK TO UPPER ;;
    # Parameter ;;
    pushl $BUFFER_DATA          # push location of buffer
    pushl %eax                  # push size of buffer, read call returns
                                # number of bytes read by system
    call convert_to_upper       # call converter function
    popl %eax                   # get the buffer size back
                                # READ only returns the number of bytes read
    addl $4, %esp               # reset stack position

    # WRITING BLOCK TO OUTPUT FILE;;
    movl %eax, %edx             # size of buffer
    movl $SYS_WRITE, %eax       # write sys call
    movl ST_FD_OUT(%ebp), %ebx  # filename
    movl $BUFFER_DATA, %ecx

    int $LINUX_SYSCALL

    jmp read_loop_begin

end_loop:
    # CLOSE FILES ;;
    # NOTE:    no need for error checking
    #          as errors do not mean much here

    movl $SYS_CLOSE, %EAX
    movl ST_FD_OUT(%ebp), %eb$x
    int $LINUX_SYSCALL

    movl $SYS_CLOSE, %eax
    movl ST_FD_IN(%ebp), %ebx
    int $LINUX_SYSCALL

    # EXIT ;;
    movl $SYS_EXIT, %eax
    movl $0, %ebx
    int $LINUX_SYSCALL

    # PURPOSE: function does the conversion
    #
    #
    #
    # INPUT:    first param - location of block
    #                        of memory
    #           second param - length of buffer
    #
    # OUTPUT:   will overwrite current buffer with
    #           upper-case conversion
    #
    # VARIABLES:
    #          %eax - beginnning of buffer
    #          %ebx - length of buffer
    #          %edi - current buffer offset
    #          %cl  - current byte being examined
    #                  (first part of %ecx)

    # COSTANTS ;;
    # boundery of the seatch
    .equ LOWERCASE_A, 'a'
    .equ LOWERCASE_Z, 'z'
    # conversions
    .equ UPPER_CONVERSION, 'A' - 'a'

    # STACK STUFF ;;
    .equ ST_BUFFER_LEN, 8       # Length of buffer
    .equ ST_BUFFER, 12          # Actual buffer

convert_to_upper:
    pushl %ebp
    movl %esp, %ebp

    # SET VARIABLES ;;
    movl ST_BUFFER(%ebp), %eax
    movl ST_BUFFER_LEN(%ebp), %ebx

    movl $0, %edi               # index in buffer

    # if buffer is zero length
    #      LEAVE
    cmpl $0, %ebx
    je end_convert_loop

convert_loop:
    # get current byte
    movb (%eax, %edi, 1), %cl

    # go to next byte unless between a-z
    cmpb $LOWERCASE_A, %cl
    jl next_byte
    cmpb $LOWERCASE_Z, %cl
    jg next_byte

    # OTHERWISE convert to uppercase
    addb $UPPER_CONVERSION, %cl
    # STORE new upper byte
    movb %cl, (%eax, %edi, 1)

next_byte:
    incl %edi                   # increment index
    cmpl %edi, %ebx             # check if reached end of buffer length
    jne convert_loop            # if not loop

end_convert_loop:
    # no return value as buffer modified
    movl %ebp, %esp
    popl %ebp
ret


    # PURPOSE: write string to stdout
    #
    #
    #
    # INPUT:
    #       text            - first argument
    #       length of text  - second argument
    #
    # OUTPUT:
    #       returns nothing
    #

    # STACK STUFF
    .equ TEXT, 8
    .equ LEN, 12

error:
    movl $SYS_WRITE, %eax
    movl $STD_OUT, %ebx
    movl $error_msg, %ecx
    movl $error_msg_len, %edx

    int $LINUX_SYSCALL

    movl $SYS_EXIT, %eax
    movl $1, %ebx

    int $LINUX_SYSCALL
