    ;; ASM subroutine for 16-bit checksum
    ;;
    ;; Call as;
    ;;          int ChecksumFile( unsigned int Handle, unsigned int *Checksum )
    ;;
    ;; where:
    ;;          Handle = handle # under which file to checksum is open
    ;;          Checksum = pointer to unsigned int var Checksum
    ;;
    ;; parameter structure:

    ST_ARGC     equ 0
    ST_ARGV_0   equ 4
    HANDLE      equ 8
    CHECKSUM    equ 12


    .section .data

    .section .bss
TempWord: resb 2

    .section .text
    global ChecksumFile
ChecksumFile:
    push ebp
    mov ebp, esp

    mov ebx, [ebp + HANDLE]
    sub
