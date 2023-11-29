    .text
    .align  2

#ifdef __APPLE__
    .extern _qt_error
#else
    .extern qt_error
#endif
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
#ifdef __APPLE__
    adrp x0, _qt_error@GOTPAGE
    ldr  x0, [x0, _qt_error@GOTPAGEOFF]
    br   x0
#else
    b qt_error
#endif

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

    // The stack pointer gets decreased
    // to save values of registers on the stack
    sub sp, sp, 160

    // Callee-saved
    stp x29, x30, [sp, #144]  // frame, link
    stp x27, x28, [sp, #128]  // userf, only
    stp x25, x26, [sp, #112]  // pu, pt
    stp x23, x24, [sp, #96]
    stp x21, x22, [sp, #80]
    stp x19, x20, [sp, #64]


    // float, SIMD
    stp d14, d15, [sp, #48]
    stp d12, d13, [sp, #32]
    stp d10, d11, [sp, #16]
    stp d8,  d9,  [sp, #0]

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
    ldp x29, x30, [sp, #144]    // frame, link
    ldp x27, x28, [sp, #128]    // userf, only
    ldp x25, x26, [sp, #112]    // pu, pt
    ldp x23, x24, [sp, #96]
    ldp x21, x22, [sp, #80]
    ldp x19, x20, [sp, #64]

    // float, SIMD
    ldp d14, d15, [sp, #48]
    ldp d12, d13, [sp, #32]
    ldp d10, d11, [sp, #16]
    ldp d8,  d9,  [sp, #0]

    add sp, sp, 160

_qt_align:
qt_align:
    ret

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
