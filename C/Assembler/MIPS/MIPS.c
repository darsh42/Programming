#include "MIPS.h"

void init();

MC assemble_R_type(ins_t opcode, char *line);
MC assemble_I_type(ins_t opcode, char *line);
MC assemble_J_type(ins_t opcode, char *line);

int get_mneumonic(char *line, char *mneumonic);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./as <mips.s> [*args]\n");
        return 1;
    }

    FILE *source = fopen(*(++argv), "r");

    char line[LINEBUF];

    init();
    while (fgets(line, LINEBUF, source) != NULL) {
        int opnum;
        char mneumonic[MNEUMONIC_LEN];
        int bytes_read = get_mneumonic(line, (char *) mneumonic);

        // if blank line skip
        if (bytes_read == 0)
            continue;


        for (opnum = 0; strcmp(mneumonic, optable[opnum].mneumonic) && opnum < OPCOUNT; opnum++)
            ;

        MC code;
        switch(optable[opnum].type) {
            case('R'): code = assemble_R_type(optable[opnum], line); break;
            case('I'): code = assemble_I_type(optable[opnum], line); break;
            case('J'): code = assemble_J_type(optable[opnum], line); break;
        }

        printf("%08x\n", code.code);
    }

    return 0;
}

void init() {

    /* R-Type instructions */
    optable[ 0] = (ins_t) {    "add", 'R', "rd", "rs", "rt", 0b100000};
    optable[ 1] = (ins_t) {   "addu", 'R', "rd", "rs", "rt", 0b100001};
    optable[ 2] = (ins_t) {    "and", 'R', "rd", "rs", "rt", 0b100100};
    optable[ 3] = (ins_t) {  "break", 'R', NULL, NULL, NULL, 0b001101};
    optable[ 4] = (ins_t) {    "div", 'R', "rs", "rt", NULL, 0b011010};
    optable[ 5] = (ins_t) {   "divu", 'R', "rs", "rt", NULL, 0b011011};
    optable[ 6] = (ins_t) {   "jalr", 'R', "rd", "rs", NULL, 0b001001};
    optable[ 7] = (ins_t) {     "jr", 'R', "rs", NULL, NULL, 0b001000};
    optable[ 8] = (ins_t) {   "mfhi", 'R', "rd", NULL, NULL, 0b010000};
    optable[ 9] = (ins_t) {   "mflo", 'R', "rd", NULL, NULL, 0b010010};
    optable[10] = (ins_t) {   "mthi", 'R', "rs", NULL, NULL, 0b010001};
    optable[11] = (ins_t) {   "mtlo", 'R', "rs", NULL, NULL, 0b010011};
    optable[12] = (ins_t) {   "mult", 'R', "rs", "rt", NULL, 0b011000};
    optable[13] = (ins_t) {  "multu", 'R', "rs", "rt", NULL, 0b011001};
    optable[14] = (ins_t) {    "nor", 'R', "rd", "rs", "rt", 0b100111};
    optable[15] = (ins_t) {     "or", 'R', "rd", "rs", "rt", 0b100101};
    optable[16] = (ins_t) {    "sll", 'R', "rd", "rt", "sa", 0b000000};
    optable[17] = (ins_t) {   "sllv", 'R', "rd", "rt", "rs", 0b000100};
    optable[18] = (ins_t) {    "slt", 'R', "rd", "rs", "rt", 0b101010};
    optable[19] = (ins_t) {   "sltu", 'R', "rd", "rs", "rt", 0b101011};
    optable[20] = (ins_t) {    "sra", 'R', "rd", "rt", "sa", 0b000011};
    optable[21] = (ins_t) {   "srav", 'R', "rd", "rt", "rs", 0b000111};
    optable[22] = (ins_t) {    "srl", 'R', "rd", "rt", "sa", 0b000010};
    optable[23] = (ins_t) {   "srlv", 'R', "rd", "rt", "rs", 0b000110};
    optable[24] = (ins_t) {    "sub", 'R', "rd", "rs", "rt", 0b100010};
    optable[25] = (ins_t) {   "subu", 'R', "rd", "rs", "rt", 0b100011};
    optable[26] = (ins_t) {"syscall", 'R', NULL, NULL, NULL, 0b001100};
    optable[27] = (ins_t) {    "xor", 'R', "rd", "rs", "rt", 0b100110};


    /* I-Type instructions */
    optable[28] = (ins_t) { "ddi", 'I', "rt",      "rs",   "imm", 0b001000};
    optable[29] = (ins_t) {"addi", 'I', "rt",      "rs",   "imm", 0b001001};
    optable[30] = (ins_t) {"andi", 'I', "rt",      "rs",   "imm", 0b001100};
    optable[31] = (ins_t) { "beq", 'I', "rs",      "rt", "label", 0b000100};
    optable[32] = (ins_t) {"bgez", 'I', "rs",   "label",    NULL, 0b000001}; // rt = 00001
    optable[33] = (ins_t) {"bgtz", 'I', "rs",   "label",    NULL, 0b000111}; // rt = 00000
    optable[34] = (ins_t) {"blez", 'I', "rs",   "label",    NULL, 0b000110}; // rt = 00000
    optable[35] = (ins_t) {"bltz", 'I', "rs",   "label",    NULL, 0b000001}; // rt = 00000
    optable[36] = (ins_t) { "bne", 'I', "rs",      "rt", "label", 0b000101};
    optable[37] = (ins_t) {  "lb", 'I', "rt", "imm(rs)",    NULL, 0b100000};
    optable[38] = (ins_t) { "lbu", 'I', "rt", "imm(rs)",    NULL, 0b100100};
    optable[39] = (ins_t) {  "lh", 'I', "rt", "imm(rs)",    NULL, 0b100001};
    optable[40] = (ins_t) { "lhu", 'I', "rt", "imm(rs)",    NULL, 0b100101};
    optable[41] = (ins_t) { "lui", 'I', "rt",     "imm",    NULL, 0b001111};
    optable[42] = (ins_t) {  "lw", 'I', "rt", "imm(rs)",    NULL, 0b100011};
    optable[43] = (ins_t) {"lwc1", 'I', "rt", "imm(rs)",    NULL, 0b110001};
    optable[44] = (ins_t) { "ori", 'I', "rt",      "rs",   "imm", 0b001101};
    optable[45] = (ins_t) {  "sb", 'I', "rt", "imm(rs)",    NULL, 0b101000};
    optable[46] = (ins_t) {"slti", 'I', "rt",      "rs",   "imm", 0b001010};
    optable[47] = (ins_t) {"slti", 'I', "rt",      "rs",   "imm", 0b001011};
    optable[48] = (ins_t) {  "sh", 'I', "rt", "imm(rs)",    NULL, 0b101001};
    optable[49] = (ins_t) {  "sw", 'I', "rt", "imm(rs)",    NULL, 0b101011};
    optable[50] = (ins_t) {"swc1", 'I', "rt", "imm(rs)",    NULL, 0b111001};
    optable[51] = (ins_t) {"xori", 'I', "rt",      "rs",   "imm", 0b001110};


    /* J-Type instructions */
    optable[52] = (ins_t) {  "j", 'J', "label", NULL, NULL, 0b000010}; // coded address of label
    optable[52] = (ins_t) {"jal", 'J', "label", NULL, NULL, 0b000011}; // coded address of label
}

int get_label(char *token);

int get_register(char *token) {
    if (strcmp(token, "zero") == 0) return  0;
    if (strcmp(token, "at") == 0)   return  1;
    if (strcmp(token, "v1") == 0)   return  2;
    if (strcmp(token, "v3") == 0)   return  3;
    if (strcmp(token, "a0") == 0)   return  4;
    if (strcmp(token, "a1") == 0)   return  5;
    if (strcmp(token, "a2") == 0)   return  6;
    if (strcmp(token, "a3") == 0)   return  7;
    if (strcmp(token, "t0") == 0)   return  8;
    if (strcmp(token, "t1") == 0)   return  9;
    if (strcmp(token, "t2") == 0)   return 10;
    if (strcmp(token, "t3") == 0)   return 11;
    if (strcmp(token, "t4") == 0)   return 12;
    if (strcmp(token, "t5") == 0)   return 13;
    if (strcmp(token, "t6") == 0)   return 14;
    if (strcmp(token, "t7") == 0)   return 15;
    if (strcmp(token, "s0") == 0)   return 16;
    if (strcmp(token, "s1") == 0)   return 17;
    if (strcmp(token, "s2") == 0)   return 18;
    if (strcmp(token, "s3") == 0)   return 19;
    if (strcmp(token, "s4") == 0)   return 20;
    if (strcmp(token, "s5") == 0)   return 21;
    if (strcmp(token, "s6") == 0)   return 22;
    if (strcmp(token, "s7") == 0)   return 23;
    if (strcmp(token, "t8") == 0)   return 24;
    if (strcmp(token, "t9") == 0)   return 25;
    if (strcmp(token, "k0") == 0)   return 26;
    if (strcmp(token, "k1") == 0)   return 27;
    if (strcmp(token, "gp") == 0)   return 28;
    if (strcmp(token, "sp") == 0)   return 29;
    if (strcmp(token, "s8") == 0)   return 30;
    if (strcmp(token, "ra") == 0)   return 31;
    return -1;
}

int set_arg(char *token, char *set_arg, args_t *args) {
    if (strcmp(set_arg, "rd") == 0) {
        (*args).rd = get_register(token);
        return 0;
    }

    if (strcmp(set_arg, "rs") == 0) {
        (*args).rs = get_register(token);
        return 0;
    }

    if (strcmp(set_arg, "rt") == 0) {
        (*args).rt = get_register(token);
        return 0;
    }

    if (strcmp(set_arg, "sa") == 0) {
        (*args).sa = get_register(token);
        return 0;
    }

    // TODO: Implement a symbol table and allow for label linking
    if (strcmp(set_arg, "label") == 0) {
        (*args).target = 0xffffff;
        return 0;
    }

    if (strcmp(set_arg, "imm") == 0) {
        (*args).imm = strtol(token, NULL, 0);
        return 0;
    }

    if (strcmp(set_arg, "imm(rs)") == 0) {
        // NOTE: the reg and imm value are in same argument imm(rs)
        char imm[LINEBUF];
        char rs[10];

        int t = 0;
        for (int i = 0; token[i] != '\0'; i++) {
            if (token[i] == ')') {
                rs[i - t] = '\0';
                break;
            }

            if (token[i] == '(') {
                t = i + 1;
                imm[i] = '\0';
            }

            if (t == 0) imm[i] = token[i];
            else        rs[i - t] = token[i];
        }

        // split reg and imm into separate
        (*args).rs = get_register((char *)rs);
        (*args).imm = strtol(imm, NULL, 0);


        return 0;
    }

    return 1;
}

int get_mneumonic(char *line, char *mneumonic) {
    int i, t;

    // skip white space
    for (i = 0; line[i] == ' ' && i < LINEBUF; i++)
        ;

    // store start of mneumonic
    for (t = i; line[i] != ' ' && line[i] != ';' && i < t + MNEUMONIC_LEN - 1 && i <LINEBUF; i++)
        mneumonic[i - t] = line[i];

    // append NULL char
    mneumonic[i - t] = '\0';

    return i - t;
}

int get_args(char *line,  args_t *args, ins_t instruction) {
    int i, t;
    int arg_count = 0;
    // skip whitespace
    for (i = 0; line[i] == ' ' && i < LINEBUF; i++)
        ;

    t = i;
    char token[LINEBUF];
    // BUG: token count will be wrong as the loop will start again after hitting , and SPACE
    for(int token_count = 0; line[i] != COMMENT && i < LINEBUF && token_count < 4; token_count++) {
        // get the next text block
        for (; line[i] != ' ' && line[i] != ',' && line[i] != ':' && line[i] != '\n' && line[i] != ';'; i++)
                token[i - t] = line[i];

        int status = 0;
        // if not skip mneumonic
        if (!(token_count == 0 && token[i] != ':')) {
            token[i - t] = '\0';
            switch (token_count) {
                case(0): // Label
                        // TODO: AddToSymbolTabel
                    break;
                case(1): // Destination
                    if (instruction.dest != NULL) {
                        status = set_arg(token, instruction.dest, args);
                        break;
                    }
                    // else fall through to next argument
                case(2): // Argument 1
                    if (instruction.arg1 != NULL) {
                        status = set_arg(token, instruction.arg1, args);
                        break;
                    } // fall through to next argument
                case(3): // Argument 2
                    if (instruction.arg2 != NULL) {
                        status = set_arg(token, instruction.arg2, args);
                    }
                    break;
            }
        }

        if (status != 0)
            fprintf(stderr, "[Error] MIPS.c: Error loading correct argument, Recieved: %s\n", token);

        if (line[i] == ',')
            i++;

        i++;
        t = i;
    }

    return arg_count;
}

MC assemble_R_type(ins_t instruction, char *line) {
    MC assembly;
    args_t args = {0,0,0,0,0,0};
    get_args(line, &args, instruction);

    assembly.R_Type.fn = instruction.fn_op;
    assembly.R_Type.sa = args.sa;
    assembly.R_Type.rd = args.rd;
    assembly.R_Type.rt = args.rt;
    assembly.R_Type.rs = args.rs;
    assembly.R_Type.op = 0b00000;

    return assembly;
}

MC assemble_I_type(ins_t instruction, char *line) {
    MC assembly;
    args_t args = {0,0,0,0,0,0};
    get_args(line, &args, instruction);

    assembly.I_Type.imm = args.imm;
    assembly.I_Type.rt = args.rt;
    assembly.I_Type.rs = args.rs;
    assembly.I_Type.op = instruction.fn_op;

    // pertains to comment after instruction is added to optable
    if (strcmp(instruction.mneumonic, "bgez") == 0)
        assembly.I_Type.rt = 0b00001;

    if (strcmp(instruction.mneumonic, "bgtz") == 0)
        assembly.I_Type.rt = 0b00000;

    if (strcmp(instruction.mneumonic, "blez") == 0)
        assembly.I_Type.rt = 0b00000;

    if (strcmp(instruction.mneumonic, "bltz") == 0)
        assembly.I_Type.rt = 0b00000;

    return assembly;
}

MC assemble_J_type(ins_t instruction, char *line) {
    MC assembly;
    args_t args = {0,0,0,0,0,0};
    get_args(line, &args, instruction);

    assembly.J_Type.target = args.target;
    assembly.J_Type.op = instruction.fn_op;

    return assembly;
}
