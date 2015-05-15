cd boot
nasm boot.asm -f bin -o boot.bin

cd ../kernel

nasm kernel_entry.asm -f elf -o kernel_entry.o

gcc -ffreestanding -c kernel.c -o kernel.o -m32
# ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary --entry main
ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o -melf_i386 --oformat binary --entry main

cd ..
cat boot/boot.bin kernel/kernel.bin > os-image
