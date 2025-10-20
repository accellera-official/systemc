# QuickThreads -- Threads-building toolkit.
# Copyright (c) 1993 by David Keppel
#
# Permission to use, copy, modify and distribute this software and
# its documentation for any purpose and without fee is hereby
# granted, provided that the above copyright notice and this notice
# appear in all copies.  This software is provided as a
# proof-of-concept and for demonstration purposes; there is no
# representation about the suitability of this software for any
# purpose.
#
# 64-bit RISC-V Architecture Support
# written by Jan Henrik Weinstock, MachineWare GmbH

    .text
    .align 2

# qt_block and qt_blocki can be called from C via:
#
#    void* qt_block(qt_helper_t* h, void* a0, void* a1, qt_t* newthread);
#    void* qt_blocki(qt_helper_t* h, void* a0, void* a1, qt_t* newthread);
#
# qt_blocki should only save integer registers (not used by SystemC)
# symbols with leading "_" needed for SystemV compatiblity
    .globl _qt_blocki
    .globl qt_blocki
    .globl _qt_block
    .globl qt_block
_qt_block:
qt_block: 
_qt_blocki:
qt_blocki: 
# Allocate stack frame for 13 integer and 12 fp-registers, 16 byte aligned
    addi    sp, sp, -208

# Save non-volatile integer registers (ra, fp, s1-s11)
    sd      ra,     0(sp)
    sd      fp,     8(sp)   # s0 / frame pointer
    sd      s1,    16(sp)
    sd      s2,    24(sp)
    sd      s3,    32(sp)
    sd      s4,    40(sp)
    sd      s5,    48(sp)
    sd      s6,    56(sp)
    sd      s7,    64(sp)
    sd      s8,    72(sp)
    sd      s9,    80(sp)
    sd      s10,   88(sp)
    sd      s11,   96(sp)

# Save non-volatile floating point registers (fs0-fs11)
    fsd     fs0,  104(sp)
    fsd     fs1,  112(sp)
    fsd     fs2,  120(sp)
    fsd     fs3,  128(sp)
    fsd     fs4,  136(sp)
    fsd     fs5,  144(sp)
    fsd     fs6,  152(sp)
    fsd     fs7,  160(sp)
    fsd     fs8,  168(sp)
    fsd     fs9,  176(sp)
    fsd     fs10, 184(sp)
    fsd     fs11, 192(sp)

# Call our helper function to save the old stack in memory
    mv      t0, a0     # Save our qt_helper_t* h from arg0
    mv      a0, sp     # arg0: old thread's stack pointer
    mv      sp, a3     # we run this already on the new stack
    jalr    t0

# Restore non-volatile floating-point registers (fs0-fs11)
    fld     fs11, 192(sp)
    fld     fs10, 184(sp)
    fld     fs9,  176(sp)
    fld     fs8,  168(sp)
    fld     fs7,  160(sp)
    fld     fs6,  152(sp)
    fld     fs5,  144(sp)
    fld     fs4,  136(sp)
    fld     fs3,  128(sp)
    fld     fs2,  120(sp)
    fld     fs1,  112(sp)
    fld     fs0,  104(sp)

# Restore non-volatile integer registers (ra, fp, s1-s11)
    ld      s11,   96(sp)
    ld      s10,   88(sp)
    ld      s9,    80(sp)
    ld      s8,    72(sp)
    ld      s7,    64(sp)
    ld      s6,    56(sp)
    ld      s5,    48(sp)
    ld      s4,    40(sp)
    ld      s3,    32(sp)
    ld      s2,    24(sp)
    ld      s1,    16(sp)
    ld      fp,     8(sp)   # s0 / frame pointer
    ld      ra,     0(sp)

# Deallocate the stack frame and return to new stack
    addi    sp, sp, 208
    ret


# qt_start will be used as the return address (in ra) of our initial stack
# frame, so we have something to return to in the beginning; it then
# collects the thread init func and its arguments from the restored stack
# frame (from whichever registers those ended up in) and then jumps there
# see QUICKTHREAD_ARGS in riscv64.h
    .globl _qt_start
    .globl qt_start
_qt_start:
qt_start:
    mv      a0, s4
    mv      a1, s3
    mv      a2, s2
    jalr    s1
    call    qt_error


    .globl _qt_abort
    .globl qt_abort
_qt_abort:
qt_abort:
    call    qt_block
