# labyrinth makefile
# author: midnight-the-rock

# silent on default
SILENT=@

ifeq ($(verbose), 1)
	SILENT=
endif

# default architecture i386
ARCH=i386

# tooling
CC=$(ARCH)-elf-g++
LD=$(ARCH)-elf-ld

# flags
CCFLAGS=-D__is_kernel	\
	-lgcc		\
	-ffreestanding	\
	-nostdlib	\
	-Werror		\
	-Wextra		\
	-std=c++20	\
	-fstack-protector-all

LDFLAGS=-T $(LINKER_SCRIPT)

# directories that should be added as
# system libraries
LIBS=-I../include

# building variables and rules
OBJ_DIR=./objects/_
OBJ_FILES=

# kernel related, should provide definition to
#  OBJ_FILES and LINKER_SCRIPT
include ../kernel/makefile.inc

# emulator related stuff

QEMU_SERIAL=-serial mon:stdio
QEMU_SPEAKER=-audiodev pa,id=speaker -machine pcspk-audiodev=speaker 
QEMU_MEMORY=-m size=130

QEMU_COMMAND=qemu-system-$(ARCH) $(QEMU_SERIAL) $(QEMU_SPEAKER) $(QEMU_MEMORY) kernel.iso	

# building targets
.PHONY: clean build image run 

all: run

clean:
	@ echo -e "\n========== Cleaning =========="
	@ echo -e "\e[0;31m"

	$(SILENT) rm -rfv objects/*
	$(SILENT) rm -rfv *.iso *.bin

	@ echo -e "\e[m"

build: $(OBJ_FILES)
	@ echo -e "\n========== Linking ==========\n"

	$(SILENT) mkdir -p $(OBJ_DIR)
	$(SILENT) $(CC) $(LDFLAGS) $(LIBS) $(OBJ_FILES) $(CCFLAGS)

image: build
	@ echo -e "\n========== Generating Image  ==========\n"

	$(SILENT) cp kernel.bin isodir/boot/kernel.bin 
	$(SILENT) grub-mkrescue -o kernel.iso isodir

run: image	
	@ echo -e "\n========== Running ==========\n"

	$(SILENT) $(QEMU_COMMAND)


BUILD_MESSAGE="\e[0;32mbuilding $<\e[m"

$(OBJ_DIR)/%.cpp.o: %.cpp
	@ echo -e $(BUILD_MESSAGE)
	$(SILENT) mkdir -p $(dir $@)
	$(SILENT) $(CC) $(LIBS) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR)/%.S.o: %.S
	@ echo -e $(BUILD_MESSAGE)
	$(SILENT) mkdir -p $(dir $@)
	$(SILENT) $(CC) $(LIBS) $(CCFLAGS) -c $< -o $@
