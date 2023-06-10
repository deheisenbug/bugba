MAKE_CONF := $(wildcard Makefile.conf)

ifeq ($(MAKE_CONF),)
    $(info No Makefile.conf found. Skipping include.)
else
    include $(MAKE_CONF)
endif

CONF ::= $(if ${RELEASE},release,debug)

ifndef RELEASE
DEBUG ::= yes
endif

## DIRECTORIES

PROJDIR ::= ${PWD}
BBASE ::= build
BDIR ::= ${BBASE}/${CONF}
BIDIR ::= ${BDIR}
CDBDIR ::= ${BDIR}/cdb
DEPDIR ::= ${BDIR}/dep
ASSETDIR ::= assets
SDIR ::= src
IDIR ::= inc
LDIR ::= lib
LOGDIR ::= log
SCRIPTDIR ::= script

## TOOLS/COMPILER

AR ::= arm-none-eabi-ar
LD ::= arm-none-eabi-ld

ifdef USE_GCC
CC ::= arm-none-eabi-gcc
CP ::= arm-none-eabi-g++
OC ::= arm-none-eabi-objcopy
else
CC ::= clang
CP ::= clang++
OC ::= llvm-objcopy
endif

## TOOLS/OTHER

EMU ?= mgba
EMUFL ::= -4

ifdef OGBIOS
	EMUFL += -b /data/jmk/bin/games/console/gba/GBA.BIOS
endif

GRIT ?= grit
GRITFL ::= -ff

LLDB ?= lldb
LLDBFL ::=

GDB_INI ?= gdb.ini

GDB ?= arm-none-eabi-gdb
GDBFL ::= -d ${SCRIPTDIR} -x ${GDB_INI}

TERMEMU ?= alacritty
TERMEMUFL ::= -e

## TOOL FLAGS

GCC_SYSROOT ::= $(shell arm-none-eabi-gcc -mcpu=arm7tdmi -print-sysroot 2>&1)
GCC_MULTIDIR ::= $(shell arm-none-eabi-gcc -mcpu=arm7tdmi -print-multi-directory 2>&1)
GCC_LIBDIR ::= ${GCC_SYSROOT}/lib/${GCC_MULTIDIR}

GCC_LIBSEARCHDIR ?= /usr/lib/gcc/arm-none-eabi/13.1.0/ 
GCC_LIBINCDIRS ?= -I/usr/arm-none-eabi/include/c++/13.1.0/ -I/usr/arm-none-eabi/include/c++/13.1.0/arm-none-eabi/

### COMPILER AND ASSEMBLER FLAGS
CXASFL ::= -mcpu=arm7tdmi -Wall -Werror -I${IDIR} -I${BIDIR}
#CXASFL += $(if ${QUIET},,-v)
CXASFL += -Wextra
CXASFL += -nostdlib
#CXASFL += -nostdinc
CXASFL += -Wshadow
CXASFL += -Wundef

ifdef USE_GCC
else
CXASFL += --target=armv4t-unknown-none-eabi
endif

ifdef DEBUG
#	CXASFL += -g -O0
#	CXASFL += -ggdb3 -Os
#	CXASFL += -ggdb -Os
#TODO: temporary fix, as clangs dwarfv5 somehow does not get accepted by gdb
	CXASFL += -gdwarf-4 -Os
else
	CXASFL += -Os
endif

ifdef VERBOSE
	CXASFL += -v
endif

### CX COMPILER FLAGS
CXFL ::= ${CXASFL}
#TODO: this didn't fix stuff. write own libc/memcopy? or use another implementation for baremetal
#CXFL += -fno-short-enums
#CXFL += -fno-builtin
#CXFL += -ffast-math
#CXFL += -ffunction-sections
#CXFL += -fdata-sections
#TODO: evaluate no-common
#CXFL += -fno-common
#TODO: check clang stack-usage option
CXFL += -fstack-usage
#TODO: investigate how -sysroot works
CXFL += --sysroot=${GCC_SYSROOT}
CXFL += -I${GCC_LIBDIR}
CXFL += -I/usr/arm-none-eabi/include/
# Only necessary when not including standard path with (-nostdinc)
#CXFL += -I/usr/lib/gcc/arm-none-eabi/13.1.0/include/

### DEPENDENCY GENERATION FLAGS
CXDEPS ::= -MMD -MP -MF

### COMPILER DATABASE FLAGS
ifdef USE_GCC
else
CXCDB = -MJ ${CDBDIR}/$(notdir $@).part.json
endif

### ADDITIONAL C++ COMPILER FLAGS
CPFL ::= -std=c++20 -fno-exceptions -fno-unwind-tables -fno-rtti
CPFL += -fno-threadsafe-statics
#CPFL += -ffreestanding #why no work? #TODO
#CPFL += -fmodules
CPFL += ${GCC_LIBINCDIRS}

### ADDITIONAL C COMPILER FLAGS
CCFL ::=

### ASSEMBLER FLAGS
ASFL ::= ${CXASFL}

### LINKER FLAGS
LDFL ::= -nostdlib -Tlink.ld -L${GCC_LIBSEARCHDIR} -L/usr/arm-none-eabi/lib/

ifdef VERBOSE
	LDFL += --verbose
endif

### ARCHIVER FLAGS
ARFL ::=

### TARGETS

PROGS ::= example_lcd_pixel pengfly

RUNS ::= $(addprefix run_,${PROGS})
GDBS ::= $(addprefix gdb_,${PROGS})
DBGS ::= $(addprefix dbg_,${PROGS})

all: ${PROGS}

${PROGS}: % :${BDIR}/%.bin

${RUNS}: run_%: % | ${LOGDIR}
	${EMU} ${EMUFL} ${BDIR}/$*.bin 1>${LOGDIR}/${EMU}_$*.out 2>${LOGDIR}/${EMU}_$*.err &

${GDBS}: EMUFL += -g
${GDBS}: gdb_%: run_% ${BDIR}/%.elf
	sleep 2 && ${TERMEMU} ${TERMEMUFL} ${GDB} ${GDBFL} ${BDIR}/$*.elf &

${DBGS}: EMUFL += -d
${DBGS}: dbg_%: % | ${LOGDIR}
	${TERMEMU} ${TERMEMUFL} ${EMU} ${EMUFL} ${BDIR}/$*.bin 1>${LOGDIR}/${EMU}.out 2>${LOGDIR}/${EMU}.err &

# PHONY

clean:
	rm -rf ./$(BDIR)

cleanall:
	rm -rf ./$(BBASE)/*

.PHONY: clean cdb ${PROGS} ${RUNS} ${DBGS} ${GDBS} ${BINS}
.PRECIOUS: ${BDIR}/%.s ${BDIR}/%.o ${BDIR}/%.elf ${BDIR}/%.bin

-include ${DEPDIR}/*.d

### SRC

SRC_COMM_S ::= rt0.s
SRC_COMM_C ::=

OBJ_COMM ::= $(addprefix ${BDIR}/,${SRC_COMM_S:.s=.o} ${SRC_COMM_C:.c=.o})

### MISC

${BDIR} ${CDBDIR} ${DEPDIR} ${LOGDIR} ${SDIR}:
	mkdir -p $@

cdb: ${BBASE}/compile_commands.json
${BBASE}/compile_commands.json: ${CDBDIR}/*.part.json ${CDBDIR} | ${BBASE}
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' ${CDBDIR}/*.part.json > ${BBASE}/compile_commands.json

### TARGET SPECIFIC

OBJ_ADD ::=
SCR_ADD ::=
INC_ADD ::=
ASSETS ::=

pengfly: ASSETS += penguin_spriteByElthen tile_bg_ice tile_title_default apple_sprite windows_sprite android_sprite chrome_sprite tile_title_retry tile_title_win

# Generate the target specific variables for each binary
define generate_deps
$(1): OBJ_ADD += $$(patsubst %, $${BDIR}/%.o, $${ASSETS})
$(1): INC_ADD += $$(patsubst %, %.h, $${ASSETS})
$(1): INC_DEP += $$(patsubst %, $${BIDIR}/%.h, $${ASSETS})
endef
$(foreach prog, $(PROGS), $(eval $(call generate_deps,$(prog))))

### PATTERN
.SECONDEXPANSION: # second expansion is used to add additional objects as prerequisites for specific targets. is this a good idea?

# C compilation rule macro/function
define compile_cc
	${CC} ${CXFL} ${CXDEPS} ${DEPDIR}/$(notdir $@).d ${CXCDB} ${CCFL} $(addprefix -include ,${INC_ADD}) -S $< -o $@
endef

# CPP compilation rule macro/function
define compile_cp
	${CP} ${CXFL} ${CXDEPS} ${DEPDIR}/$(notdir $@).d ${CXCDB} ${CPFL} $(addprefix -include ,${INC_ADD}) -S $< -o $@
endef

# C source compilation rules
${BDIR}/%.s: ${SDIR}/%.c Makefile | ${BDIR} ${DEPDIR} ${CDBDIR}
	$(compile_cc)

# CPP source compilation rules
${BDIR}/%.s: ${SDIR}/%.cpp Makefile | ${BDIR} ${DEPDIR} ${CDBDIR}
	$(compile_cp)

# Generated CPP source compilation rules per binary
define generate_cprule
$${BDIR}/$(1).s: ${SDIR}/$(1).cpp Makefile $$$${INC_DEP} | $${BDIR} $${DEPDIR} $${CDBDIR}
	$$(compile_cp)
endef
$(foreach prog, $(PROGS), $(eval $(call generate_cprule,$(prog))))

# Asset conversion rules
${BDIR}/%.s ${BIDIR}/%.h: ${ASSETDIR}/%.bmp ${ASSETDIR}/%.grit | ${BDIR}
	(cd ${BDIR} && ${GRIT} ${PROJDIR}/$< ${GRITFL})

define assemble_as
	${CC} ${ASFL} ${CCFL} -c $< -o $@
endef

# Assemlbing rules for build directory
${BDIR}/%.o: ${BDIR}/%.s Makefile | ${BDIR}
	$(assemble_as)
#	${CC} ${ASFL} ${CCFL} -c $< -o $@

# Assemlbing rules for source directory
${BDIR}/%.o: ${SDIR}/%.s Makefile | ${BDIR}
	$(assemble_as)
#	${CC} ${ASFL} ${CCFL} -c $< -o $@

# OBJCOPY
${BDIR}/%.bin: ${BDIR}/%.elf Makefile | ${BDIR}
	${OC} -O binary $< $@

# Linking rule macro/function
define link_ld
	${LD} ${LDFL} -Map ${<:.o=.map} ${OBJ_COMM} ${OBJ_ADD} $< -lc -lgcc -o $@
endef

# Basic linking rule
${BDIR}/%.elf: ${BDIR}/%.o ${OBJ_COMM} Makefile link.ld mem_ioregs.ld | ${BDIR}
	$(link_ld)

# Generated linking rules per binary
define generate_linkrule
$${BDIR}/$(1).elf: $${BDIR}/$(1).o $${OBJ_COMM} $$$${OBJ_ADD} Makefile link.ld mem_ioregs.ld | $${BDIR}
	$$(link_ld)
endef
$(foreach prog, $(PROGS), $(eval $(call generate_linkrule,$(prog))))
