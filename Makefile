MAKE_CONF ::= $(wildcard Makefile.conf)

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
EXTDIR ::= extern

## TOOLS/COMPILER

AR ::= arm-none-eabi-ar
LD ::= arm-none-eabi-ld

ifeq ($(USE_GCC),1)
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
	EMUFL += -b ${OGBIOS}
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

## Some paths to host libraries/headers, defaults for current arch (based) linux
## should be set in Makefile.conf
HOST_LDFL_LIBSEARCH ?= -L/usr/lib/gcc/arm-none-eabi/13.1.0/
HOST_CPFL_LIBINC ?= -I/usr/arm-none-eabi/include/c++/13.1.0/ -I/usr/arm-none-eabi/include/c++/13.1.0/arm-none-eabi/
HOST_CXFL_STDINC ?= -I/usr/lib/gcc/arm-none-eabi/13.1.0/include/ -I/usr/lib/clang/15.0.7/include/
HOST_LIBCPPDIR ?= /usr/include/c++/v1/
#HOST_GCC_LIBDIR ?= /usr/arm-none-eabi/include/

## Use agbabi and LLVM's C++ standard library
ifeq ($(USE_AGBABI_LIBCPP),1)
LDFL_LIBSEARCH ?= -L${EXTDIR}/agbabi/build/${CONF}/
CPFL_LIBINC ?= -I${HOST_LIBCPPDIR} -D_LIBCPP_HAS_NO_THREADS
LDFL_LATE = -lagbabi
else
## Use gcc's c/c++ standard library
LDFL_LIBSEARCH = ${HOST_LDFL_LIBSEARCH}
CPFL_LIBINC = ${HOST_CPFL_LIBINC}
LDFL_LATE = -lc -lgcc
endif

GCC_LIBDIR ?= ${HOST_GCC_LIBDIR}
CXFL_STDINC ?= ${HOST_CXFL_STDINC}

## SYSROOT STUFF

GCC_SYSROOT ::= $(shell arm-none-eabi-gcc -mcpu=arm7tdmi -print-sysroot 2>&1)
ifeq ($(GCC_SYSROOT),)
$(Info GCC_SYSROOT could not be determined. disabling)
override USE_GCC_SYSROOT ::=
else
GCC_MULTIDIR ::= $(shell arm-none-eabi-gcc -mcpu=arm7tdmi -print-multi-directory 2>&1)
ifeq ($(GCC_LIBDIR),)
GCC_LIBDIR = ${GCC_SYSROOT}/lib/${GCC_MULTIDIR}
endif
endif

### COMPILER AND ASSEMBLER FLAGS
CXASFL ::= -mcpu=arm7tdmi -Wall -Werror -I${IDIR} -I${BIDIR}
#CXASFL += $(if ${QUIET},,-v)
CXASFL += -Wextra
CXASFL += -nostdlib
CXASFL += -Wshadow
CXASFL += -Wundef

ifeq ($(USE_GCC),1)
else
CXASFL += --target=armv4t-unknown-none-eabi
endif

ifeq ($(DEBUG),1)
#	CXASFL += -g -O0
#	CXASFL += -ggdb3 -Os
#	CXASFL += -ggdb -Os
#TODO: temporary fix, as clangs dwarfv5 somehow does not get accepted by gdb
	CXASFL += -gdwarf-4 -Os
else
	CXASFL += -Os
endif

ifeq ($(VERBOSE),1)
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
ifeq ($(USE_GCC_SYSROOT),1)
CXFL += --sysroot=${GCC_SYSROOT}
else
CXFL += -I${GCC_LIBDIR}
endif
#CXFL += -I/usr/arm-none-eabi/include/
##Debug includes
#CXFL += -H
#TODO: evaluate -nostdinc
#CXFL += -nostdinc
# Only necessary when not including standard path with (-nostdinc)
#CXFL += ${CXFL_STDINC}

### DEPENDENCY GENERATION FLAGS
CXDEPS ::= -MMD -MP -MF

### COMPILER DATABASE FLAGS
ifeq ($(USE_GCC),1)
else
CXCDB = -MJ ${CDBDIR}/$(notdir $@).part.json
endif

### ADDITIONAL C++ COMPILER FLAGS
CPFL ::= -std=c++20 -fno-exceptions -fno-unwind-tables -fno-rtti
CPFL += -fno-threadsafe-statics
#CPFL += -ffreestanding #TODO: investigate if freestanding is feasable/usefull?
#CPFL += -fmodules
CPFL += ${CPFL_LIBINC}

### ADDITIONAL C COMPILER FLAGS
CCFL ::=

### ASSEMBLER FLAGS
ASFL ::= ${CXASFL}

### LINKER FLAGS
LDFL ::= -nostdlib -Tlink.ld ${LDFL_LIBSEARCH} -L/usr/arm-none-eabi/lib/

ifeq ($(VERBOSE),1)
	LDFL += --verbose
endif

### ARCHIVER FLAGS
ARFL ::=

### TARGETS

PROGS ::= example_lcd_pixel example_paint pengfly

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

cleanlib:
	rm -rf ./${EXTDIR}/agbabi/build/${CONF}

cleanliball:
	rm -rf ./${EXTDIR}/agbabi/build

.PHONY: clean cleanall cleanlib cleanliball cdb ${PROGS} ${RUNS} ${DBGS} ${GDBS} ${BINS}
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

pengfly_ASSETS ::= penguin_spriteByElthen tile_bg_ice tile_title_default apple_sprite windows_sprite android_sprite chrome_sprite tile_title_retry tile_title_win

# Generate the target specific variables for each binary
define generate_deps
$(1)_OBJ_ADD ::= $(patsubst %, ${BDIR}/%.o, $(value $(1)_ASSETS))
$(1)_INC_ADD ::= $(patsubst %, %.h, $(value $(1)_ASSETS))
$(1)_INC_DEP ::= $(patsubst %, ${BIDIR}/%.h, $(value $(1)_ASSETS))
endef
$(foreach prog, $(PROGS), $(eval $(call generate_deps,$(prog))))

### PATTERN

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
$$(eval $(1) : INC_ADD = $$(value $(1)_INC_ADD))
$${BDIR}/$(1).s: $${SDIR}/$(1).cpp Makefile $$(value $(1)_INC_DEP) | $${BDIR} $${DEPDIR} $${CDBDIR}
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
	${LD} ${LDFL} -Map ${<:.o=.map} ${OBJ_COMM} ${OBJ_ADD} $< ${LDFL_LATE} -o $@
endef
#-lc -lgcc 

# Basic linking rule
${BDIR}/%.elf: ${BDIR}/%.o ${OBJ_COMM} Makefile link.ld mem_ioregs.ld ${EXTDIR}/agbabi/build/${CONF}/libagbabi.a | ${BDIR}
	$(link_ld)

# Generated linking rules per binary
define generate_linkrule
$$(eval $(1) : OBJ_ADD = $$(value $(1)_OBJ_ADD))
$${BDIR}/$(1).elf: $${BDIR}/$(1).o $${OBJ_COMM} $$(value $(1)_OBJ_ADD) Makefile link.ld mem_ioregs.ld $${EXTDIR}/agbabi/build/$${CONF}/libagbabi.a | $${BDIR}
	$$(link_ld)
endef
$(foreach prog, $(PROGS), $(eval $(call generate_linkrule,$(prog))))

## AGBABI specific targets

# Create Makefile
${EXTDIR}/agbabi/build/${CONF}/Makefile:
	cd ${EXTDIR}/agbabi && cmake -S . -B build/${CONF} --toolchain=cross/agb.cmake -DCMAKE_BUILD_TYPE=${CONF}
##-DCMAKE_C_FLAGS_${CONF}="${CXASFL}"

# Build library
${EXTDIR}/agbabi/build/${CONF}/libagbabi.a: ${EXTDIR}/agbabi/build/${CONF}/Makefile
	cd ${EXTDIR}/agbabi && cmake --build build/${CONF}
