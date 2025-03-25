#ifndef QUICKTHREADS_RISCV64_H
#define QUICKTHREADS_RISCV64_H

// For QUICKTHREADS_SPUT in qt.h
typedef unsigned long qt_word_t;

extern void qt_start(void);
extern void qt_vstart(void);

extern void qt_error(void);
extern void qt_align(void);

// Stack must be 16-byte aligned at all times.
#define QUICKTHREADS_STKALIGN (16)
#define QUICKTHREADS_STKBASE  (26 * sizeof(qt_word_t))
#define QUICKTHREADS_VSTKBASE (5 * sizeof(qt_word_t))

#define QUICKTHREADS_GROW_DOWN
#define QUICKTHREADS_ADJ(sp)  (((char *)sp) - QUICKTHREADS_STKBASE)

// Initialize a single-argument thread.
//
// Set up link index to point to the true initializer, qt_start,
// which will call only(pu, pt, userf) read from the stack.
//
// (*helper)(old_sp, a0, a1) will be called in between on the current
// stack, so FRAME_INDEX needs to be correct and the other values
// appropriately stored.
#define QUICKTHREADS_ARGS(sp, pu, pt, userf, only)                               \
   (QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_LINK_INDEX,  qt_start), \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_FRAME_INDEX, sp),       \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_ONLY_INDEX,  only),     \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_USER_INDEX,  userf),    \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_ARGT_INDEX,  pt),       \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_ARGU_INDEX,  pu),       \
    ((qt_t *)QUICKTHREADS_ADJ(sp)))

#define QUICKTHREADS_LINK_INDEX  0 // ra; standard
#define QUICKTHREADS_FRAME_INDEX 1 // fp; standard
#define QUICKTHREADS_ONLY_INDEX  2 // s1; arbitrary
#define QUICKTHREADS_USER_INDEX  3 // s2; arbitrary
#define QUICKTHREADS_ARGT_INDEX  4 // s3; arbitrary
#define QUICKTHREADS_ARGU_INDEX  5 // s4; arbitrary

#endif // QUICKTHREADS_RISCV64_H
