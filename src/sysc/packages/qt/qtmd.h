#if defined( __sparc ) || defined( __sparc__ )
#include "sysc/packages/qt/md/sparc.h"
#elif defined( __hppa )
#include "sysc/packages/qt/md/hppa.h"
#elif defined( __x86_64__ )
#include "sysc/packages/qt/md/iX86_64.h"
#elif defined( __i386 )
#include "sysc/packages/qt/md/i386.h"
#elif defined( __ppc__ )
#include "sysc/packages/qt/md/powerpc_mach.h"
#elif defined( __powerpc )
#include "sysc/packages/qt/md/powerpc_sys5.h"
#elif defined( __aarch64__ )
#include "sysc/packages/qt/md/aarch64.h"
#endif
