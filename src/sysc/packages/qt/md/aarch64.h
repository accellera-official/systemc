#ifndef QUICKTHREADS_AARCH_64_H
#define QUICKTHREADS_AARCH_64_H

// For QUICKTHREADS_SPUT in qt.h
typedef unsigned long qt_word_t;

extern void qt_start(void);
extern void qt_vstart(void);

extern void qt_error(void);
extern void qt_align(void);


// Stack must be 16-byte aligned at all times.
#define QUICKTHREADS_STKALIGN (16)
#define QUICKTHREADS_STKBASE  (20 * sizeof(qt_word_t))
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
#define QUICKTHREADS_ARGS(sp, pu, pt, userf, only) \
   (QUICKTHREADS_SPUT(sp, QUICKTHREADS_LINK_INDEX,  qt_start), \
    QUICKTHREADS_SPUT(sp, QUICKTHREADS_FRAME_INDEX, sp),       \
    QUICKTHREADS_SPUT(sp, QUICKTHREADS_ONLY_INDEX,  only),     \
    QUICKTHREADS_SPUT(sp, QUICKTHREADS_USER_INDEX,  userf),    \
    QUICKTHREADS_SPUT(sp, QUICKTHREADS_ARGT_INDEX,  pt),       \
    QUICKTHREADS_SPUT(sp, QUICKTHREADS_ARGU_INDEX,  pu),       \
    ((qt_t *)QUICKTHREADS_ADJ(sp)))

#define QUICKTHREADS_LINK_INDEX  -1 // x30; standard
#define QUICKTHREADS_FRAME_INDEX -2 // x29; standard
#define QUICKTHREADS_ONLY_INDEX  -3 // x28; arbitrary
#define QUICKTHREADS_USER_INDEX  -4 // x27; arbitrary
#define QUICKTHREADS_ARGT_INDEX  -5 // x26; arbitrary
#define QUICKTHREADS_ARGU_INDEX  -6 // x25; arbitrary

#endif // QUICKTHREADS_AARCH_64_H
