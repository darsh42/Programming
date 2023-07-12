[org 0x7c00]

input:
    ; 0 identifier for keyboard input
    mov ah, 0
    ; index ofset start at 0
    mov bx, buffer
    ;; current input size
    mov cx, 0

inputLoop:
    ;; check input size
    cmp cx,100
    je printString

    ; BIOS interrupt for input
    int 0x16

    ; check backspace
    cmp al, 0x10
    je backspace

    ; check newline
    cmp al, 0x12
    je printString

    ; move char in addr buffer + bx
    mov [bx], al
    ; increment offset
    inc bx
    inc cx
    ; loop
    jmp inputLoop

backspace:
    ; check start of buffer
    cmp bx, 0
    je input

    dec bx
    dec cx
    jmp inputLoop


printString:
    mov ah, 0x0e
    mov bx, buffer
printLoop:
    mov al, [bx]
    cmp al, 0
    je end
    int 0x10
    inc bx
    jmp printLoop

end:
    jmp $

buffer:
    times 100 db 0, 0

    times 510-($-$$) db 0
    db 0x55, 0xaa
