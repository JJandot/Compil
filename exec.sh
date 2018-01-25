#!/bin/bash
reset
make all
./compilateur -n test.l
nasm -f elf -g -F dwarf assembleur.txt -o assembleur.o
ld -m elf_i386 -o assembleur assembleur.o
./assembleur
