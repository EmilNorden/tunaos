ASM_KERNEL_SOURCES = $(wildcard kernel/*.asm)
C_KERNEL_SOURCES = $(wildcard kernel/*.c drivers/*.c)
ASM_BOOT_SOURCES = $(wildcard boot/*.asm)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
GCC_FLAGS = -D ENABLE_ASSERTIONS

OBJ = ${C_KERNEL_SOURCES:.c=.o}
ASM_KERNEL_OBJ = ${ASM_KERNEL_SOURCES:.asm=.o}

MKDIR_P = mkdir -p
BUILD_DIR = build

# Default make target
all: build_dir ${BUILD_DIR}/os-image

.PHONY: build_dir
build_dir: ${BUILD_DIR}
		
${BUILD_DIR}:
	${MKDIR_P} ${BUILD_DIR}

${BUILD_DIR}/os-image: ${BUILD_DIR}/boot_sect.bin ${BUILD_DIR}/kernel.bin
	cat $^ > ${BUILD_DIR}/os-image

# Build the kernel binary
${BUILD_DIR}/kernel.bin: kernel/kernel_entry.o ${OBJ} ${ASM_KERNEL_OBJ}
	ld -o $@ -Ttext 0x1000 -melf_i386 $^ --oformat binary --entry main
# is 0x1912 as high as I can go without crashes?

# Build the kernel object file
%.o: %.c ${HEADERS}
	gcc -Wpedantic ${GCC_FLAGS} -std=c11 -ffreestanding -m32 -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: ${ASM_BOOT_SOURCES}
	nasm $< -f bin -I 'boot/' -o $@ 
	
run-qemu: all
	qemu-system-x86_64 ${BUILD_DIR}/os-image

run-bochs: all
	bochs -f bochsrc -q
	
usb: all
	 dd if=${BUILD_DIR}/os-image of=/dev/sdd1

clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin driver/*.o

