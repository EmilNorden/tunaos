cmake_minimum_required(VERSION 2.6)
PROJECT (os)

# SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

SET (CMAKE_C_FLAGS "-m32 -ffreestanding -fno-exceptions -nostdlib -nostdinc -fno-stack-protector")

ENABLE_LANGUAGE (ASM_NASM)

FILE (GLOB_RECURSE os_ASM_SOURCES "boot/*.asm")
FILE (GLOB_RECURSE os_C_SOURCES "kernel/*.c")

ADD_LIBRARY (os.bin STATIC
	${os_ASM_SOURCES}
	${os_C_SOURCES})