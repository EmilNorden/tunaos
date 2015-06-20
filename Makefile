ASM_KERNEL_SOURCES = $(wildcard kernel/*.asm)
C_KERNEL_SOURCES = $(wildcard kernel/*.c drivers/*.c)
ASM_BOOT_SOURCES = $(wildcard boot/*.asm)
ASM_LOADER_SOURCES = $(wildcard boot/loader/*.asm)
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

${BUILD_DIR}/os-image: ${BUILD_DIR}/boot_sect.bin ${BUILD_DIR}/boot_loader.bin ${BUILD_DIR}/kernel.bin
	cat $^ > ${BUILD_DIR}/os-image
	
${BUILD_DIR}/boot_loader.bin: boot/loader/loader.asm
	nasm $< -f bin -I 'boot/loader/' -o ${BUILD_DIR}/boot_loader.bin

# Build the kernel binary
${BUILD_DIR}/kernel.bin: kernel/kernel_entry.o ${OBJ} ${ASM_KERNEL_OBJ}
	ld -o $@ -Ttext 0x1600 -melf_i386 $^ --oformat binary --entry main

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
	rm -fr ${BUILD_DIR}/*.bin ${BUILD_DIR}/*.dis ${BUILD_DIR}/*.o ${BUILD_DIR}/os-image
	rm -fr kernel/*.o boot/*.bin driver/*.o

