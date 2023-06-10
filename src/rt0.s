.arm
.global __start

__start:
    b .Linit

    @ this is replaced with correct header info by `gbafix`
    .space 188

.Linit:
    @ Set address of user IRQ handler
    @ldr r0, =MainIrqHandler
    @mov r1, #0x04000000
    @str r0, [r1, #-4]

    @ set IRQ stack pointer
    mov r0, #0x12
    msr CPSR_c, r0
    ldr sp, =0x03007FA0

    @ set user stack pointer
    mov r0, #0x1f
    msr CPSR_c, r0
    ldr sp, =0x03007F00

    @ Sets WAITCNT to the default used by GBA games
    @
    @ See https://problemkaputt.de/gbatek.htm#gbasystemcontrol for reference.
    ldr r0, =0x04000204
    ldr r1, =0x4317
    str r1, [r0]

    @ copy .data and .text_iwram section to IWRAM
    ldr r0, =__iwram_lma     @ source address
    ldr r1, =__iwram_start   @ destination address
    ldr r2, =__iwram_end
    subs r2, r1             @ length
    @ these instructions are only executed if r2 is nonzero
    @ (i.e. don't bother copying an empty .iwram section)
    addne r2, #3
    asrne r2, #2
    addne r2, #0x04000000
    swine 0xB0000

    @ jump to user code
    @ ldr r0, =main
    bl main
    @ main should be `fn() -> !`, but it doesn't hurt to guard
    1: b 1b
