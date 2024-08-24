#include "instructions.h"

MC assemble_R_type(int fn, int sa, int rd, int rt, int rs);
MC assemble_I_type(int imm, int rt, int rs, int fn);
MC assemble_J_type(int target, int fn);

MC assemble_R_type(int fn, int sa, int rd, int rt, int rs) {
    MC assembly;

    assembly.R_Type.fn = fn;
    assembly.R_Type.sa = sa;
    assembly.R_Type.rd = rd;
    assembly.R_Type.rt = rt;
    assembly.R_Type.rs = rs;
    assembly.R_Type.op = 0b00000;

    printf("%08x\n", assembly.code);

    return assembly;
}

MC assemble_I_type(int imm, int rt, int rs, int fn) {
    MC assembly;

    assembly.I_Type.imm = imm;
    assembly.I_Type.rt = rt;
    assembly.I_Type.rs = rs;
    assembly.I_Type.op = fn;

    printf("%08x\n", assembly.code);

    return assembly;
}

MC assemble_J_type(int target, int fn) {
    MC assembly;

    assembly.J_Type.target = target;
    assembly.J_Type.op = fn;

    printf("%08x\n", assembly.code);

    return assembly;
}
