ARCH_CC_FILES=$(wildcard $(ARCH_DIR)/*.cpp)
ARCH_AS_FILES=$(wildcard $(ARCH_DIR)/*.S)

ARCH_CC_FILES+=$(wildcard $(ARCH_DIR)/memory/*.cpp)
ARCH_AS_FILES+=$(wildcard $(ARCH_DIR)/memory/*.S)

CRT_BEGIN=$(OBJ_DIR)/$(ARCH_DIR)/runtime/crti.S.o
CRT_BEGIN+=$(shell $(ARCH)-elf-g++ -print-file-name=crtbegin.o) 

CRT_END=$(shell $(ARCH)-elf-g++ -print-file-name=crtend.o)
CRT_END+=$(OBJ_DIR)/$(ARCH_DIR)/runtime/crtn.S.o

OBJ_FILES+=$(ARCH_CC_FILES) $(ARCH_AS_FILES)
