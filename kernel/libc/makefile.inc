LIBC_CC_FILES=$(wildcard $(LIBC_DIR)/*/*.cpp)
LIBC_AS_FILES=$(wildcard $(LIBC_DIR)/*/*.S)

OBJ_FILES+=$(LIBC_CC_FILES) $(LIBC_AS_FILES)
