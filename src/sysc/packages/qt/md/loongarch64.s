    .text
    .align  2

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

_qt_block:
qt_block:
_qt_blocki:
qt_blocki:
    addi.d  $sp, $sp, -160

    st.d    $ra,  $sp, 0
    st.d    $fp,  $sp, 8
    st.d    $s0,  $sp, 16
    st.d    $s1,  $sp, 24
    st.d    $s2,  $sp, 32
    st.d    $s3,  $sp, 40
    st.d    $s4,  $sp, 48
    st.d    $s5,  $sp, 56
    st.d    $s6,  $sp, 64
    st.d    $s7,  $sp, 72
    st.d    $s8,  $sp, 80

    fst.d   $fs0, $sp, 88
    fst.d   $fs1, $sp, 96
    fst.d   $fs2, $sp, 104
    fst.d   $fs3, $sp, 112
    fst.d   $fs4, $sp, 120
    fst.d   $fs5, $sp, 128
    fst.d   $fs6, $sp, 136
    fst.d   $fs7, $sp, 144

    or      $t0, $a0, $zero
    or      $a0, $sp, $zero
    or      $sp, $a3, $zero
    jirl    $ra, $t0, 0

    fld.d   $fs7, $sp, 144
    fld.d   $fs6, $sp, 136
    fld.d   $fs5, $sp, 128
    fld.d   $fs4, $sp, 120
    fld.d   $fs3, $sp, 112
    fld.d   $fs2, $sp, 104
    fld.d   $fs1, $sp, 96
    fld.d   $fs0, $sp, 88

    ld.d    $s8,  $sp, 80
    ld.d    $s7,  $sp, 72
    ld.d    $s6,  $sp, 64
    ld.d    $s5,  $sp, 56
    ld.d    $s4,  $sp, 48
    ld.d    $s3,  $sp, 40
    ld.d    $s2,  $sp, 32
    ld.d    $s1,  $sp, 24
    ld.d    $s0,  $sp, 16
    ld.d    $fp,  $sp, 8
    ld.d    $ra,  $sp, 0

    addi.d  $sp, $sp, 160

_qt_align:
qt_align:
    jirl    $zero, $ra, 0

_qt_start:
qt_start:
    or      $a0, $s3, $zero
    or      $a1, $s2, $zero
    or      $a2, $s1, $zero
    jirl    $ra, $s0, 0
    bl      qt_error

_qt_abort:
qt_abort:
    b       qt_block

#if defined(__linux__) && defined(__ELF__)
    .section .note.GNU-stack,"",@progbits
#endif
