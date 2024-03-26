#!/usr/bin/env python3
import os
from ctypes import *


class Assembler:
    def __init__(self, source:str, binary:str):
        """ C lib path """
        self._lib_ins  = os.path.join(os.getcwd(), "libins.so")
        self._lib = CDLL(self._lib_ins)

        """ Files """
        self.source = source
        self.binary = binary

        """ Tables """
        self.symtable = []
        self.optable = []

        """ Options """
        self.COMMENT_SYM = '#'

        """ R-Type instructions """
        self.optable.append([    "add", "rd", "rs", "rt", 0b100000, 'R'])
        self.optable.append([   "addu", "rd", "rs", "rt", 0b100001, 'R'])
        self.optable.append([    "and", "rd", "rs", "rt", 0b100100, 'R'])
        self.optable.append([  "break", None, None, None, 0b001101, 'R'])
        self.optable.append([    "div", "rs", "rt", None, 0b011010, 'R'])
        self.optable.append([   "divu", "rs", "rt", None, 0b011011, 'R'])
        self.optable.append([   "jalr", "rd", "rs", None, 0b001001, 'R'])
        self.optable.append([     "jr", "rs", None, None, 0b001000, 'R'])
        self.optable.append([   "mfhi", "rd", None, None, 0b010000, 'R'])
        self.optable.append([   "mflo", "rd", None, None, 0b010010, 'R'])
        self.optable.append([   "mthi", "rs", None, None, 0b010001, 'R'])
        self.optable.append([   "mtlo", "rs", None, None, 0b010011, 'R'])
        self.optable.append([   "mult", "rs", "rt", None, 0b011000, 'R'])
        self.optable.append([  "multu", "rs", "rt", None, 0b011001, 'R'])
        self.optable.append([    "nor", "rd", "rs", "rt", 0b100111, 'R'])
        self.optable.append([     "or", "rd", "rs", "rt", 0b100101, 'R'])
        self.optable.append([    "sll", "rd", "rt", "sa", 0b000000, 'R'])
        self.optable.append([   "sllv", "rd", "rt", "rs", 0b000100, 'R'])
        self.optable.append([    "slt", "rd", "rs", "rt", 0b101010, 'R'])
        self.optable.append([   "sltu", "rd", "rs", "rt", 0b101011, 'R'])
        self.optable.append([    "sra", "rd", "rt", "sa", 0b000011, 'R'])
        self.optable.append([   "srav", "rd", "rt", "rs", 0b000111, 'R'])
        self.optable.append([    "srl", "rd", "rt", "sa", 0b000010, 'R'])
        self.optable.append([   "srlv", "rd", "rt", "rs", 0b000110, 'R'])
        self.optable.append([    "sub", "rd", "rs", "rt", 0b100010, 'R'])
        self.optable.append([   "subu", "rd", "rs", "rt", 0b100011, 'R'])
        self.optable.append(["syscall", None, None, None, 0b001100, 'R'])
        self.optable.append([    "xor", "rd", "rs", "rt", 0b100110, 'R'])


        """ I-Type instructions """
        # TODO: Fix the indexed representation
        self.optable.append([ "ddi", "rt",      "rs",   "imm", 0b001000, 'I'])
        self.optable.append(["addi", "rt",      "rs",   "imm", 0b001001, 'I'])
        self.optable.append(["andi", "rt",      "rs",   "imm", 0b001100, 'I'])
        self.optable.append([ "beq", "rs",      "rt", "label", 0b000100, 'I'])
        self.optable.append(["bgez", "rs",   "label",    None, 0b000001, 'I']) # rt = 00001
        self.optable.append(["bgtz", "rs",   "label",    None, 0b000111, 'I']) # rt = 00000
        self.optable.append(["blez", "rs",   "label",    None, 0b000110, 'I']) # rt = 00000
        self.optable.append(["bltz", "rs",   "label",    None, 0b000001, 'I']) # rt = 00000
        self.optable.append([ "bne", "rs",      "rt", "label", 0b000101, 'I'])
        self.optable.append([  "lb", "rt", "imm(rs)",    None, 0b100000, 'I'])
        self.optable.append([ "lbu", "rt", "imm(rs)",    None, 0b100100, 'I'])
        self.optable.append([  "lh", "rt", "imm(rs)",    None, 0b100001, 'I'])
        self.optable.append([ "lhu", "rt", "imm(rs)",    None, 0b100101, 'I'])
        self.optable.append([ "lui", "rt",     "imm",    None, 0b001111, 'I'])
        self.optable.append([  "lw", "rt", "imm(rs)",    None, 0b100011, 'I'])
        self.optable.append(["lwc1", "rt", "imm(rs)",    None, 0b110001, 'I'])
        self.optable.append([ "ori", "rt",      "rs",   "imm", 0b001101, 'I'])
        self.optable.append([  "sb", "rt", "imm(rs)",    None, 0b101000, 'I'])
        self.optable.append(["slti", "rt",      "rs",   "imm", 0b001010, 'I'])
        self.optable.append(["slti", "rt",      "rs",   "imm", 0b001011, 'I'])
        self.optable.append([  "sh", "rt", "imm(rs)",    None, 0b101001, 'I'])
        self.optable.append([  "sw", "rt", "imm(rs)",    None, 0b101011, 'I'])
        self.optable.append(["swc1", "rt", "imm(rs)",    None, 0b111001, 'I'])
        self.optable.append(["xori", "rt",      "rs",   "imm", 0b001110, 'I'])


        """ J-Type instructions """
        self.optable.append([  "j", "label", None, None, 0b000010, 'J']) # coded address of label
        self.optable.append(["jal", "label", None, None, 0b000011, 'J']) # coded address of label


    def compile(self) -> None:
        with open(self.source, "r") as source:
            for LC, line in enumerate(source):

                tokens = self.parse_line(LC, line)

                if len(tokens) == 0: continue

                opcode = self.optable[tokens[0].value]
                print(f"{opcode}")

                mc = self.assemble(opcode, tokens)

                # print(f"{LC}:{hex(mc)}")

        return

    def assemble(self, opcode: list, args: list) -> int:
        mc: int
        fn     = opcode[-2]
        sa     = args[opcode.index("sa")]     if "sa"     in args else 0
        rd     = args[opcode.index("rd")]     if "rd"     in args else 0
        rt     = args[opcode.index("rt")]     if "rt"     in args else 0
        rs     = args[opcode.index("rs")]     if "rs"     in args else 0
        imm    = args[opcode.index("imm")]    if "imm"    in args else 0
        target = args[opcode.index("target")] if "target" in args else 0

        print(f"FN:{fn}, SA:{sa}, RD:{rd}, RT:{rt}, RS:{rs}, IMM:{imm}, target:{target}")

        match(opcode[-1]):
            case 'R':
                mc = self._lib.assemble_R_type(c_int(fn), c_int(sa), c_int(rd), c_int(rt), c_int(rs))
            case 'I':
                mc = self._lib.assemble_I_type(c_int(imm), c_int(rt), c_int(rs), c_int(fn))
            case 'J':
                mc = self._lib.assemble_J_type(c_int(target), c_int(fn))
        return mc

    def parse_line(self, LC:int, line:str) -> list:
        tokens = []

        line = line.replace('(', ' ')
        line = line.replace(')', ' ')
        line = line.replace(',', ' ')
        line = line.strip()

        symbols = line.split()

        for count, symbol in enumerate(symbols):
            token = Token()
            if (';' in symbol):
                break;
            if (count == 0):
                token.mneumonic(self, symbol)
                tokens.append(token)
                continue
            if('$' in symbol):
                tokens.append(token.register(symbol))
                continue
            if (symbol.isnumeric):
                tokens.append(token.immediate(symbol))
                continue
            if ('.' in symbol):
                tokens.append(token.directive(self, symbol))
                continue
            if (':' in symbol):
                token.new_symbol(self, LC)

            tokens.append(token.symbol(self, LC, symbol))

        return tokens

class Token:
    def __init__(self):
        self.value      :int = 0
        self.token_type :str = ""

    def register(self, value:str) -> None:
        self.token_type = "register"

        value = value.replace('$', '')
        match value:
            case "zero": self.value = 0;
            case "at":   self.value =  1;
            case "v1":   self.value =  2;
            case "v3":   self.value =  3;
            case "a0":   self.value =  4;
            case "a1":   self.value =  5;
            case "a2":   self.value =  6;
            case "a3":   self.value =  7;
            case "t0":   self.value =  8;
            case "t1":   self.value =  9;
            case "t2":   self.value = 10;
            case "t3":   self.value = 11;
            case "t4":   self.value = 12;
            case "t5":   self.value = 13;
            case "t6":   self.value = 14;
            case "t7":   self.value = 15;
            case "s0":   self.value = 16;
            case "s1":   self.value = 17;
            case "s2":   self.value = 18;
            case "s3":   self.value = 19;
            case "s4":   self.value = 20;
            case "s5":   self.value = 21;
            case "s6":   self.value = 22;
            case "s7":   self.value = 23;
            case "t8":   self.value = 24;
            case "t9":   self.value = 25;
            case "k0":   self.value = 26;
            case "k1":   self.value = 27;
            case "gp":   self.value = 28;
            case "sp":   self.value = 29;
            case "s8":   self.value = 30;
            case "ra":   self.value = 31;

        return self

    def mneumonic(self, A:Assembler, value:str) -> None:
        self.token_type = "mneumonic"
        for i, op in enumerate(A.optable):
            if (value in op):
                self.value = i
                break

        return self

    def immediate(self, value:str) -> None:
        self.token_type = "immediate"
        self.value = int(value)
        return self

    def directive(self, A:Assembler, value: str) -> None:
        self.token_type = "directive"
        self.value = A.directives.index(value)
        return self

    def symbol(self, A:Assembler, value:str) -> None:
        if value in A.symtable:
            self.value = A.symtable.index(value)
        else:
            A.add_symbol(value)
            self.symbol(value)

        return self



assembler = Assembler("example.s", "example")
assembler.compile()
