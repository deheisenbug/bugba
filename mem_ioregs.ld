
mem_io_test = 0x04000000 ;
mem_vram_test = 0x06000000 ;
m3pix1 = mem_vram_test ;
display = mem_io_test ;

/* memory regiion symbol definitions */
mem_pal = 0x05000000 ;
mem_vram = 0x06000000 ;
mem_oam = 0x07000000 ;

/* control register symbol definitions */
reg_disp = 0x04000000 ;
reg_snd = 0x04000060 ;
reg_key = 0x04000130 ;
reg_irq = 0x04000200 ;
reg_pwr = 0x04000300 ;

/* misc register and region definitions */
irq_vector = 0x03007FFC;

/* BIOS REGISTERS in IWRAM? (why?) */

bios_irqflags = 0x03007FF8;
