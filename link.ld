INCLUDE mem_ioregs.ld

ENTRY(__start)

MEMORY {
    ewram (w!x) : ORIGIN = 0x2000000, LENGTH = 256K
    iwram (w!x) : ORIGIN = 0x3000000, LENGTH = 32K
    rom (rx)    : ORIGIN = 0x8000000, LENGTH = 32M
}

SECTIONS {
    .text : {
        KEEP(*rt0.o(.text));
        *(.text .text.*);
        . = ALIGN(4);
    } >rom = 0xff

    .rodata : {
        *(.rodata .rodata.*);
        . = ALIGN(4);
    } >rom = 0xff

    __iwram_lma = .;
    .iwram : {
        __iwram_start = ABSOLUTE(.);
        
        __data_start = ABSOLUTE(.);
        *(.data .data.*);
        . = ALIGN(4);
        __data_end = ABSOLUTE(.);
        
        __text_iwram_start = ABSOLUTE(.);
        *(.text_iwram .text_iwram.*);
        . = ALIGN(4);
        __text_iwram_end = ABSOLUTE(.);
        
        __iwram_end = ABSOLUTE(.);
    } >iwram AT>rom = 0xff

    .bss : {
        *(.bss .bss.*);
        . = ALIGN(4);
        __bss_end = ABSOLUTE(.);
    } >iwram

    /* debugging sections */
    /* Stabs */
    .stab            0 : { *(.stab) }
    .stabstr         0 : { *(.stabstr) }
    .stab.excl       0 : { *(.stab.excl) }
    .stab.exclstr    0 : { *(.stab.exclstr) }
    .stab.index      0 : { *(.stab.index) }
    .stab.indexstr   0 : { *(.stab.indexstr) }
    .comment         0 : { *(.comment) }
    /* DWARF 1 */
    .debug           0 : { *(.debug) }
    .line            0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo   0 : { *(.debug_srcinfo) }
    .debug_sfnames   0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges   0 : { *(.debug_aranges) }
    .debug_pubnames  0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info      0 : { *(.debug_info) }
    .debug_abbrev    0 : { *(.debug_abbrev) }
    .debug_line      0 : { *(.debug_line) }
    .debug_frame     0 : { *(.debug_frame) }
    .debug_str       0 : { *(.debug_str) }
    .debug_loc       0 : { *(.debug_loc) }
    .debug_macinfo   0 : { *(.debug_macinfo) }
    /* DWARF ??: TODO: sort for correct dwarf version */
    .debug_line_str 0 : { *(.debug_line_str) }
    .debug_line_str_offset 0 : { *(.debug_line_str_offset) }
    .debug_line_strp 0 : { *(.debug_line_strp) }
    .debug_ranges 0 : { *(.debug_ranges) }
    .debug_types 0 : { *(.debug_types) }
    .debug_pubtypes 0 : { *(.debug_pubtypes) }
    .debug_pubaddr 0 : { *(.debug_pubaddr) }
    .debug_pubmacro 0 : { *(.debug_pubmacro) }
    .debug_rnglists 0 : { *(.debug_rnglists) }
    .debug_loclists 0 : { *(.debug_loclists) }
    .debug_str_offsets 0 : { *(.debug_loclists) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
    
    /* keep .shstrtab as ld.lld complains about discarding it */    
    .shstrtab 0 : { *(.shstrtab) }

    /* discard anything not already mentioned */
    /DISCARD/ : { *(*) }
}
