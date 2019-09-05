#!/bin/bash

# 1. What does the following statement do?
#A: To specify target ARM processor "arm926ej-s", then assemble ts.s to ts.o(-g generate debug information)
arm-none-eabi-as -mcpu=arm926ej-s -g ts.s -o ts.o

# 2. What does the following statement do?
#A: To specify target ARM processor "arm926ej-s", then compile t.c to t.o
arm-none-eabi-gcc -c -mcpu=arm926ej-s -g t.c -o t.o

arm-none-eabi-gcc -c string.c

ar -r strlib string.o #create static library strlib

arm-none-eabi-nm strlib

# 3. What does the following statement do?
#A: t.ld is used to specify the entry point and the lay out of progrm section 
#   Then link ts.o, t.o and strlib to t.elf
arm-none-eabi-ld -T t.ld ts.o t.o strlib -o t.elf

# 4. What does the following statement do?
#A: Objectcopy t.elf to t.bin
arm-none-eabi-objcopy -O binary t.elf t.bin

rm *.o *.elf

echo ready to go?
read dummy

qemu-system-arm -M realview-pbx-a9 -m 128M -kernel t.bin \
-serial mon:stdio -serial /dev/pts/1 -serial /dev/pts/2 -serial /dev/pts/3

 
 
 



 





