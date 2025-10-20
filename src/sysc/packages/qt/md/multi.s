/* multi.s -- arm64,i386,x86_64 assembly support. */

#if defined(__aarch64__) || defined(__arm64__)
#include "aarch64.s"
#elif defined(__i386)
#include "i386.s"
#elif defined(__x86_64__)
#include "iX86_64.s"
#endif
