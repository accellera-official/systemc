/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  stack_alignment.cpp -- This example shows the crash of an fxsave instruction 
                         in the sc_thread stack environment, but not in the 
                         original linux process stack, which is correctly 
                         aligned on first function.
                         
  Please note that this test probably runs OK on a faulty implementation in 
  64-bit in general (depending on your libc implementation), but will crash 
  for sure in 32-bit.

  Original Author: Eric Paire, STMicroelectronics

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

/*
 * This program exhibits a bug in the management by QT of the stack of each
 * SystemC process. At least on i686 & x86_64, GCC makes the assumption that
 * the stack is aligned on a 16-byte boundary on each C/C++ function entry.
 * This convention allows GCC to respects constraints of automatic (stack)
 * variable alignment, using the __attribute)__ ((align(X))) GCC extension.
 *
 * The X is known to be 16 for i686 & x86_64, as this is the largest alignment
 * required by instructions operands, actually used by fxsave instruction.
 *
 * The attached code shows up the problem by crashing when fxsave is executed
 * in a SystemC thread, and executing correctly the *same* code on the initial
 * process stack, as initialized by the libc runtime.
 *
 * This misbehavior does not occur systematically for x86_64 (no crash,
 * or crash difficult to reproduce with standard malloc()), but often does
 * with i686. Notice that the instruction with the right alignment is shown
 * when using the myfpxregs address which is aligned on 16-byte boundary.
 */

#if defined(__x86_64__)
#  define FXSAVE "fxsaveq"
#else
#  define FXSAVE "fxsave"
#endif

#if defined(__GNUC__)
#  define ALIGNED_ARRAY( Type, Name, Size, Align ) \
	  Type Name[Size] __attribute__((aligned(Align)))
#elif defined(_MSC_VER)
#  define ALIGNED_ARRAY( Type, Name, Size, Align ) \
      __declspec(align(Align)) Type Name[Size]
#endif

#if defined(__GNUC__) && ( defined(__x86_64__) || defined(__i386__) )
#  define ASM( Assembly ) __asm__ __volatile__( Assembly )
#else
#  define ASM( Assembly ) /* not implemented */
#endif

// Class
SC_MODULE(C)
{ 
public:
    SC_CTOR(C) {
        SC_THREAD(run);
    }
    void run(void) 
    { 
        ALIGNED_ARRAY( char, fpxregs64, 512+15, 16 );

        cout << "Inside C::run() " << endl; 

        // manually enforce alignment (volatile to avoid optmizations)
        char * volatile myfpxregs = fpxregs64;
        while ((uintptr_t)myfpxregs & 0xF)
            myfpxregs++;

        // the "real" requirement: enforced alignment works
        sc_assert( !((uintptr_t)fpxregs64 & 0xF) );
        sc_assert( !((uintptr_t)myfpxregs & 0xF) );
        sc_assert( myfpxregs == fpxregs64 );

        // test assembly on supported platforms
        ASM( FXSAVE " (%0)" :: "r"(myfpxregs) );
        cout << "Between C::run() " << endl; 
        ASM( FXSAVE " %0" : "=m"(fpxregs64) );

        cout << "Out of C::run() " << endl; 
    }
};

int sc_main(int , char** ) {
  C the_C("C");

  ALIGNED_ARRAY( char, fpxregs64, 512, 16 );

  cout << "Inside sc_main() " << endl; 
  ASM( FXSAVE " %0" : "=m"(fpxregs64) );
  sc_start(1, SC_NS);
  cout << "Out of sc_main() " << endl; 
  return 0;
}
