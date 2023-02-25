#!/usr/bin/env python3

with open("instructions.txt", "r", encoding="utf-8") as file:
    data = file.readlines()


with open("untabbed.txt", "w+", encoding="utf-8") as file:
    for line in data:
        file.writelines(line.replace("\t", "    "))
