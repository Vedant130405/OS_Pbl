set -e

nasm -f elf32 boot.asm -o boot.o

# compile kernel, print.c and mm.c with correct include paths
gcc -m32 -ffreestanding -fno-stack-protector -fno-pic -fno-pie -c kernel.c -o kernel.o
gcc -m32 -ffreestanding -fno-stack-protector -fno-pic -fno-pie -c ./src/print.c -o ./src/print.o

# IMPORTANT: add -I./ so mm.c can find ./include/mm.h
gcc -m32 -I./ -ffreestanding -fno-stack-protector -fno-pic -fno-pie -c ./src/mm.c -o ./src/mm.o

# link all required objects WITH linker script
ld -m elf_i386 -nostdlib -T link.ld -o ./bin/kernel.bin boot.o kernel.o ./src/print.o ./src/mm.o

# run in QEMU (32-bit)
qemu-system-i386 -kernel ./bin/kernel.bin

