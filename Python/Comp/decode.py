#!/usr/bin/env python3
import numpy as np

coded = "ETEVHTWGSAHGWYVPNKQOEGWYVPNKPDEPHWAOVWPFWNHANEVWXAVOAEAJEUXTAOWBTEVHTWGSAHGWYVPNQAOQVGTYHAVAXETOANFQEOIQPLANTEVHFYNSQVEBEOWSKNCKLOPEVTYJAUFWYNCOTWZESQEPERQSQOPEVYCEVHEGDEHEVHEYOPNQEEHWYFTKTEVHTWGSAHGWYVPNKQOWVAPDEPWVTKFWNHANOTEVHTWGSAHGWYVPNQAOPDANAENAWVTKPIWHWYFTKTEVHTWGSAHGWYVPNQAOQVPDAIWNTHWVAWBPDAUQOYLFASQOPEVIDEPQOPDAWPDANWVA"
phrase = "NEPALSERBIASWITZERLANDBURKINAFASOKYRGYZSTANLUXEMBOURGSLOVAKIATAJIKISTANUGANDACHADANDAUSTRIA";


def find_unique(code, phr, codeIndex):
    for (value1, value2) in zip(code, phr):
        if (value1 == value2):
            return -1;

    return codeIndex

def keyGen(index):
    key = np.zeros(26, dtype=int)

    for (charCode, charHint) in zip(coded, phrase):
       if (key[ord(charCode) - ord('A')] == 0):
           key[ord(charCode) - ord('A')] = ord(charHint)

    return key

def main():
    index = 90

    key = keyGen(index)

    for i in key:
        print(chr(i))

    for codeChar in coded:
        print(chr(key[ord(codeChar) - ord('A')]), end="")



if __name__ == "__main__":
    main()
