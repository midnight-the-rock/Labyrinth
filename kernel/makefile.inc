# different locations containing parts of the kernel
# each should probably include their own isolated
# makefile.inc, to avoid humongous scripts
KERNEL_DIR=../kernel

BUS_DIR=$(KERNEL_DIR)/bus
ARCH_DIR=$(KERNEL_DIR)/arch/$(ARCH)
UTIL_DIR=$(KERNEL_DIR)/util
LIBC_DIR=$(KERNEL_DIR)/libc
MULTIBOOT_DIR=$(KERNEL_DIR)/multiboot
INTERRUPT_DIR=$(KERNEL_DIR)/interrupt

# add new system libraries 
LIBS+=-I$(KERNEL_DIR) -I$(LIBC_DIR) -I$(ARCH_DIR)

# include extra makefiles

include $(BUS_DIR)/makefile.inc
include $(ARCH_DIR)/makefile.inc
include $(UTIL_DIR)/makefile.inc
include $(LIBC_DIR)/makefile.inc
include $(MULTIBOOT_DIR)/makefile.inc
include $(INTERRUPT_DIR)/makefile.inc

# set the linker script
LINKER_SCRIPT=$(ARCH_DIR)/linker.ld

# modify OBJ_FILES, so that each <file> becomes $(OBJECT_DIR)/<file>.o
# this allows for the build script to generate a literal
# copy of the project files in object files
# which is probably a space waste but not a time waste
OBJ_FILES:=$(foreach file, $(OBJ_FILES), $(OBJ_DIR)/$(file).o)
OBJ_FILES:=$(CRT_BEGIN) $(OBJ_FILES) $(CRT_END)
