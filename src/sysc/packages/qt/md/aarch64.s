    .text
    .align  2

    .extern qt_error
    .globl _qt_start
    .globl qt_start
    .globl _qt_abort
    .globl qt_abort
    .globl _qt_block
    .globl qt_block
    .globl _qt_blocki
    .globl qt_blocki
    .globl _qt_align
    .globl qt_align


_qt_start:
qt_start:
    mov x0, x25
    mov x1, x26
    mov x2, x27
    br x28

    // The branch above should never return,
    // and there is nowhere for us to return to.
    br =qt_error

_qt_abort:
qt_abort:
_qt_block:
qt_block:
_qt_blocki:
qt_blocki:

    // 1. Store,
    // 2. Swap stacks and args,
    // 4. Call helper,
    // 3. Load,
    // 5. Return to execution.

    // Call before load because x30 (link pointer)
    // is not callee-saved.

    // Callee-saved
    stp x29, x30, [sp, #-16]  // frame, link
    stp x27, x28, [sp, #-32]
    stp x25, x26, [sp, #-48]
    stp x23, x24, [sp, #-64]
    stp x21, x22, [sp, #-80]
    stp x19, x20, [sp, #-96]

    // float, SIMD
    stp d14, d15, [sp, #-112]
    stp d12, d13, [sp, #-128]
    stp d10, d11, [sp, #-144]
    stp d8,  d9,  [sp, #-160]

    mov x4, x0  // helper = arg0
    mov x0, sp  // arg0 = old_sp
    mov sp, x3  // sp = new_sp

    // Move the stack pointer below
    // our saved data.
    sub sp, sp, 160

    // (*helper)(old_sp, a0, a1)
    blr x4

    add sp, sp, 160

    // Callee-saved
    ldp x29, x30, [sp, #-16]  // frame, link
    ldp x27, x28, [sp, #-32]
    ldp x25, x26, [sp, #-48]
    ldp x23, x24, [sp, #-64]
    ldp x21, x22, [sp, #-80]
    ldp x19, x20, [sp, #-96]

    // float, SIMD
    ldp d14, d15, [sp, #-112]
    ldp d12, d13, [sp, #-128]
    ldp d10, d11, [sp, #-144]
    ldp d8,  d9,  [sp, #-160]

_qt_align:
qt_align:
    ret
