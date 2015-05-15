cmake_minimum_required(VERSION 2.8)
PROJECT (os)

# SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

SET (CMAKE_C_FLAGS "-m32 -ffreestanding -fno-exceptions -nostdlib -nostdinc -fno-stack-protector")
SET (CMAKE_EXE_LINKER_FLAGS "-m elf_i386 -T${CMAKE_CURRENT_LIST_DIR}/link.ld")
SET (CMAKE_ASM_NASM_OBJECT_FORMAT "bin")
SET (CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -i${CMAKE_CURRENT_SOURCE_DIR}/boot")

ENABLE_LANGUAGE (ASM_NASM)

FILE (GLOB_RECURSE os_ASM_SOURCES "boot/*.asm")
FILE (GLOB_RECURSE os_C_SOURCES "kernel/*.c")
FILE (GLOB_RECURSE os_DRIVERS_C_SOURCES "drivers/*.c")



ADD_LIBRARY (os.bin STATIC
	${os_ASM_SOURCES}
	${os_C_SOURCES})
