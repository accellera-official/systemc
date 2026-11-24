#ifndef QUICKTHREADS_LOONGARCH64_H
#define QUICKTHREADS_LOONGARCH64_H

typedef unsigned long qt_word_t;

extern void qt_start(void);
extern void qt_vstart(void);
extern void qt_error(void);
extern void qt_align(void);

#define QUICKTHREADS_STKALIGN (16)
#define QUICKTHREADS_STKBASE  (20 * sizeof(qt_word_t))
#define QUICKTHREADS_VSTKBASE (5 * sizeof(qt_word_t))

#define QUICKTHREADS_GROW_DOWN
#define QUICKTHREADS_ADJ(sp)  (((char *)sp) - QUICKTHREADS_STKBASE)

#define QUICKTHREADS_ARGS(sp, pu, pt, userf, only)                               \
   (QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_LINK_INDEX,  qt_start), \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_FRAME_INDEX, sp),       \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_ONLY_INDEX,  only),     \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_USER_INDEX,  userf),    \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_ARGT_INDEX,  pt),       \
    QUICKTHREADS_SPUT(QUICKTHREADS_ADJ(sp), QUICKTHREADS_ARGU_INDEX,  pu),       \
    ((qt_t *)QUICKTHREADS_ADJ(sp)))

#define QUICKTHREADS_LINK_INDEX  0
#define QUICKTHREADS_FRAME_INDEX 1
#define QUICKTHREADS_ONLY_INDEX  2
#define QUICKTHREADS_USER_INDEX  3
#define QUICKTHREADS_ARGT_INDEX  4
#define QUICKTHREADS_ARGU_INDEX  5

#endif
