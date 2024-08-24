    ;; Does i = N*N + N*3
    lw t0, 4(gp)                ; load word to R9
    mult t0, t0                 ; multiply R9 with R9
    lw t1, 4(gp)                ; load word to R10
    ori t2, zero, 3             ; or immediate value 3 to R11
    mult t1, t2                 ; multiply R10 and R11
    add t2, t0, t1              ; add R9 and R10 and store in R11
    sw t2, 0(gp)                ; store the value in R11 in memory
