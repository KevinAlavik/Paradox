override CC := gcc
override LD := ld
override MAKEFLAGS += -rR

override KERNEL := Paradox
KERNEL_DIR := kernel
ARCH_DIR := arch/x86_64

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    override CC := x86_64-elf-gcc
    override LD := x86_64-elf-ld
endif

define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef

override DEFAULT_CC := cc
$(eval $(call DEFAULT_VAR,CC,$(DEFAULT_CC)))

override DEFAULT_LD := ld
$(eval $(call DEFAULT_VAR,LD,$(DEFAULT_LD)))

override DEFAULT_CFLAGS := -g -O2 -pipe
$(eval $(call DEFAULT_VAR,CFLAGS,$(DEFAULT_CFLAGS)))

override DEFAULT_CPPFLAGS :=
$(eval $(call DEFAULT_VAR,CPPFLAGS,$(DEFAULT_CPPFLAGS)))

override DEFAULT_NASMFLAGS := -F dwarf -g
$(eval $(call DEFAULT_VAR,NASMFLAGS,$(DEFAULT_NASMFLAGS)))

override DEFAULT_LDFLAGS :=
$(eval $(call DEFAULT_VAR,LDFLAGS,$(DEFAULT_LDFLAGS)))

override CFLAGS += \
    -O0 \
    -Ilimine \
    -Ikernel \
    -Iarch \
    -Ikernel/corelib \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIE \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mcmodel=kernel \
    -mno-80387 \
    -mno-red-zone \
    -DPRINTF_DISABLE_SUPPORT_FLOAT \
    -DHEAP_ACCESSABLE \
    -msse \
    -mgeneral-regs-only \
    -DSUPPORT_FLOAT \
    -Wimplicit-function-declaration \
    -Wdiv-by-zero \
    -Wunused-variable

override LDFLAGS += -nostdlib -static -m elf_x86_64 -z max-page-size=0x1000 -T linker.ld

override CPPFLAGS := -I. $(CPPFLAGS) -MMD -MP

override NASMFLAGS += -Wall -f elf64

override CFILES := $(shell cd $(KERNEL_DIR) && find -L * -type f -name '*.c')
override ASFILES := $(shell cd $(KERNEL_DIR) && find -L * -type f -name '*.S')
override NASMFILES := $(shell cd $(KERNEL_DIR) && find -L * -type f -name '*.asm')
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

override ARCH_CFILES := $(shell cd $(ARCH_DIR) && find -L * -type f -name '*.c')
override ARCH_ASFILES := $(shell cd $(ARCH_DIR) && find -L * -type f -name '*.S')
override ARCH_ASMFILES := $(shell cd $(ARCH_DIR) && find -L * -type f -name '*.asm')
override ARCH_OBJ := $(addprefix obj/,$(ARCH_CFILES:.c=.c.o) $(ARCH_ASFILES:.S=.S.o) $(ARCH_ASMFILES:.asm=.asm.o))
override ARCH_HEADER_DEPS := $(addprefix obj/,$(ARCH_CFILES:.c=.c.d) $(ARCH_ASFILES:.S=.S.d) $(ARCH_ASMFILES:.asm=.asm.d))

.PHONY: all
all: bin/$(KERNEL)

bin/$(KERNEL): GNUmakefile linker.ld $(OBJ) $(ARCH_OBJ)
	@printf "  LD\t$@\n"
	@mkdir -p "$$(dirname $@)"
	@$(LD) $(OBJ) $(ARCH_OBJ) $(LDFLAGS) -o $@

-include $(HEADER_DEPS) $(ARCH_HEADER_DEPS)

obj/%.c.o: $(KERNEL_DIR)/%.c GNUmakefile
	@printf "  CC\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.S.o: $(KERNEL_DIR)/%.S GNUmakefile
	@printf "  AS\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.asm.o: $(KERNEL_DIR)/%.asm GNUmakefile
	@printf "  AS\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@nasm $(NASMFLAGS) $< -o $@

obj/%.c.o: $(ARCH_DIR)/%.c GNUmakefile
	@printf "  CC\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.S.o: $(ARCH_DIR)/%.S GNUmakefile
	@printf "  AS\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.asm.o: $(ARCH_DIR)/%.asm GNUmakefile
	@printf "  AS\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@nasm $(NASMFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf bin obj Paradox.raw.bin image.iso modules/ramdisk.tar
