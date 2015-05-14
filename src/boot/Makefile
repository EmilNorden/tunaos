all:
	nasm boot.asm -f bin -o boot.bin

run:
	nasm boot.asm -f bin -o boot.bin
	qemu-system-x86_64 boot.bin
	
