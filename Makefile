C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
ASM_SOURCES = $(wildcard boot/*.asm)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}

# Default make target
all: os-image

run: all
	qemu-system-x86_64 os-image

usb: all
	 dd if=os-image of=/dev/sdd1

os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

# Build the kernel binary
kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 -melf_i386 $^ --oformat binary --entry main
# is 0x1912 as high as I can go without crashes?

# Build the kernel object file
%.o: %.c ${HEADERS}
	gcc -Wpedantic -std=c11 -ffreestanding -m32 -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: ${ASM_SOURCES}
	nasm $< -f bin -I 'boot/' -o $@ 

clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin driver/*.o