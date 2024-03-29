# Release Notes for SystemC 3.0.0

Laurent Maillet-Contoz, STMicroelectronics

Andrew C. Goodrich

## CONTENTS

  1. IMPORTANT
  
  2. What's new in this release?

  3. New features

  4. Bug fixes and enhancements

  5. Incompatibilities with previous releases

  6. C++17 support

  7. Experimental features

  8. Known problems

  9. Fixed-point library


## 1. IMPORTANT

This is the release of the SystemC 3.0.0 Class Library.
This release implements the IEEE 1666-2023 Language Reference Manual.

1. This release is supported on the following platform combinations 
   for which it has been tested:

|      OS             |    Target     |    Compiler
| ------------------- | ------------- |  --------------
| Ubuntu 20.04        |     amd64     |  gcc  9.4.0
| Ubuntu 20.04        |     arm64     |  gcc  9.4.0
| Ubuntu 22.04        |     amd64     |  gcc 11.4.0
| Ubuntu 22.04        |     arm64     |  gcc 11.4.0
| AlmaLinux 8         |     amd64     |  gcc  8.5.0
| AlmaLinux 8         |     arm64     |  gcc  8.5.0
| AlmaLinux 9         |     amd64     |  gcc 11.4.1
| AlmaLinux 9         |     arm64     |  gcc 11.4.1
| Debian 12.5         |     amd64     |  gcc 12.3.0 
| Debian 12.5         |     amd64     |  gcc 13.2.0 
| Debian 12.5         |     amd64     |  gcc 13.2.0 with --std=c++20
| Rhel 7.9            |     x86_64    |  gcc 9.3.0
| Rhel 8.8            |     amd64     |  gcc 9.4.0
| Rhel 8.8            |     amd64     |  gcc 13.1.0
| Rhel 8.8            |     amd64     |  clang 17.0.6
| Rhel 8.9            |     amd64     |  gcc 8.5.0
| Rhel 8.9            |     amd64     |  gcc 11.2.1
| Rhel 8.9            |     amd64     |  gcc 12.2.1
| Rhel 8.9            |     amd64     |  coside clang 16.0.6
| Rhel 8.9            |     amd64     |  coside gcc 13.1.0
| Windows 10          |               |  msvc16 (2019)
| Windows 11          |               |  msvc16 (2019)
| Windows 11          |               |  msvc17 (2022)
| macOS 12 (Monterey) | x86_64        | AppleClang 13 
| macOS 12 (Monterey) | arm64         | AppleClang 13
| macOS 13 (Ventura)  | x86_64        | AppleClang 14 
| macOS 13 (Ventura)  | arm64         | AppleClang 14
| macOS 14 (Sonoma)   | x86_64        | AppleClang 14
| macOS 14 (Sonoma)   | arm64         | AppleClang 14 
| macOS 14 (Sonoma)   | x86_64        | AppleClang 15
| macOS 14 (Sonoma)   | arm64         | AppleClang 15


Warning: _The fixed-point data-types are not working as defined on
MSVC 2017 and later in Release x64 mode._

This release has not been tested or is known not to work as expected
on the following formerly supported platforms:

* GNU C++ compiler versions prior to 9.3.0 (all platforms)
* FreeBSD 9.0 or later (x86, x86_64) with GNU C++ compiler
* HP-UX 11.00 or later with GNU C++ or HP C++ compiler
* Sun/Oracle Solaris, with GNU C++ or Sun/Solaris Studio compiler
* macOS prior to 11.6 (Big Sur) and/or on the x86, PowerPC architectures
* Microsoft Visual C++ versions prior to 2017 (15.0)
* Cygwin 1.7 or later (x86, x86_64) with GNU C++ compiler
* Msys/MinGW32 GNU C++ compiler

2. (In)compatibility note

While it is possible to build SystemC as a shared library (both on
ELF-based systems and well as a DLL on Windows), there is no explicitly
defined binary interface across different
  - SystemC implementations (e.g. from multiple vendors)
  - SystemC library versions
  - Compilers or compiler versions
  - Operating systems or OS versions

   Some differences might be caught at link time, while other incompatibilities 
   could just silently lead to undefined behavior during simulation. Please carefully 
   ensure a consistent build configuration, when integrating binary SystemC models built 
   from separate sources.


## 2. What's new in this release?

This version of SystemC contains the Reference Implementation simulator
for the IEEE Std. 1666-2023 SystemC standard.  Please consult the IEEE Std. 1666-2023
SystemC Language Reference Manual for details about the current SystemC standard.

Compared to the 2.3.4 release, this release has the following new items:

  - New features and API changes implemented to reflect the IEEE Std. 1666-2023
    revision, see section 3.
  
  - Bug fixes and enhancements, see section 4.

  - Deprecated features, see section 5.

  - Experimental features (some disabled by default), see section 7.
    Testing and feedback welcome via the Accellera SystemC forums
    at http://forums.accellera.org/forum/9-systemc/.
    

## 3. New features

Here is an overview of the new features available in 3.0.0.

  - New class `sc_core::sc_stage_callback_if` provides an interface to enable
    user-defined callbacks during elaboration or simulation stages that are 
    otherwise not available to the application.

  - New functions to suspend and unsuspend the simulation kernel.

  - The constructor macro `SC_CTOR` now supports additional constructor 
    parameters.

  - Class `sc_core::sc_event` introduces the member function triggered to 
    determine whether an event has recently been triggered.

  - The enumeration type `sc_core::sc_time_unit` now supports 
    attosecond (`sc_core::SC_AS`), zeptosecond (`sc_core::SC_ZS`), 
    and yoctosecond (`sc_core::SC_YS`).

  - Class `sc_core::sc_time` supports a constructor passing the time as string
    argument.

  - New class `sc_core::sc_hierarchy_scope` provides an interface to allow an 
    application to place objects of type `sc_core::sc_object` in an object 
    hierarchy outside the current hierarchical scope.

  - New functions are defined to register and un-register hierarchical names.

  - New static object `sc_core::sc_unbound` can be used in an application to 
    make an unbound (open) connection to a port.  In addition, the function 
    `sc_core::sc_tie::value(const &T)` can be used to tie a port to a specified 
    value of type `T`.

  - New definition supports tracing of types `sc_core::sc_event` and
    `sc_core::sc_time` using `sc_core::sc_trace`.

  - New functions `sc_core::sc_vector::emplace_back` and
    `sc_core::sc_vector::emplace_back_with_name` support making incremental 
    additions to `sc_core::sc_vector`.

  - Added support for C++20, lambda functions for sc_spawn and sc_bind


## 4. Bug fixes and enhancements

Following is the list of bug fixes and enhancements for the 3.0.0 release:

  - Execution speed improvements for the SystemC data-types

  - Fixed MSVC warnings in examples

  - Fixed set_stack_size issue on Windows

  - Fixed sc_vector iterator comparisons in C++20

  - Fixed Automake flow for data-types

  - Fixed various warnings and cleanup in tests

  - Improved windows support (recent Visual Studio files, tests cleanup)

  - Removed obsolete macros

  - Droped unused Boost package

  - Droped legacy coroutine stack allocation for QuickThreads 

  - Various clean-up thanks to the support of c++17 as new baseline for SystemC

  - Fixed various invalid UTF-8 charcters in comments

  - Fixed big integers issue in constructor

  - Improved CMake support

  - Merged SystemC regression tests in distribution


## 5. Deprecated features

Here is an overview of deprecated features:

  - Macro `SC_HAS_PROCESS` is not required anymore and has been deprecated

  - constructor: `sc_time(double, bool)` has been deprecated

  - constructor: `sc_time(uint64, bool)` has been deprecated

  - function: `sc_set_default_time_unit` has been deprecated

  - function: `sc_get_default_time_unit` has been deprecated 


## 6. C++17 support

IEEE Std. 1666-2023 mandates C++17 as the baseline for SystemC implementations.
`std::string_view` is used in new APIs.  However, `const char *` are still used
in several locations and will be progressively cleaned-up in the upcoming 
releases.


## 7. Experimental features

Some of these features are not enabled in the default library
configuration and need to be explicitly activated during at
library build time.  See [INSTALL.md](INSTALL.md) file.


## 8. Known Problems

  - This implementation currently truncates "small" `sc_time` objects to
    `SC_ZERO_TIME`, while "too big” objects wrap-around `sc_max_time()` and 
    lead to a value modulo the maximum time.  In both cases, no warning is 
    generated, even though such a warning is mandated by IEEE Std. 1666-2023.

  - Some typos have not been fixed yet in the version of the IEEE Std. 1666-2023 
    document available online, but have been implemented in this release:

    - the following stage callbacks are implemented
      ``` 
      void sc_register_stage_callback( sc_stage_callback_if&, int );
      void sc_unregister_stage_callback( sc_stage_callback_if&, int );
      ```
    - `static const sc_event & none()` is implemented
    - `IEEE_1666_SYSTEMC` is set to `202301L`

  - The SystemC non regression test suite still available does not take into
    account all the alignments done in the reference implementation to align
    to IEEE Std. 1666-2023.  Therefore, the non regression tests might not 
    compile or not behave as expected, this will be fixed in an upcoming 
    delivery.

  - The sign-extension of mixed-signedness logic expressions (&,|)
    involving one `sc_bigint<>` operand and C++ builtin integral types
    (int, short, etc.) is inconsistent between 32-bit and 64-bit
    platforms in some cases.  Convert both operands to `sc_bigint<>` first.

  - The definition of `sc_dt::(u)int64` differs from `std::(u)int64_t` types
    on some platforms.  This may lead to problems with function overloads
    and/or format-string placeholders.  As a workaround, convert these
    values explicitly to the correct type before passing them to functions
    expecting one of these types.  For `sc_time`, use the new nested type
    `sc_time::value_type` to hold values of the underlying representation.

  - Bit/logic-vector reductions (or_reduce, and_reduce, etc.) return an
    `sc_logic_value_t` enum value, instead of a `bool` or `sc_logic` 
    (as required as of IEEE Std. 1666-2011).  Using the return value of these
    functions in a boolean context, e.g.
    ```
      if ( lv.or_reduce() ) { /* ... */ }
    ```
    might lead to wrong results in case of `'X'` or `'Z'` bits in the vector.
    Avoid this by converting the result to an `sc_logic` first and perform
    a safe conversion to bool:
    ```
      if ( sc_logic( lv.or_reduce() ).to_bool() ) { /* ... */ }
    ```

  - The current implementation of bit-wise operations on `sc_bv` and other
    bit types implicitly uses the left-hand argument as the size of the result
    rather than the size of the larger operand, as required by the 
    IEEE Std. 1666 standard.

  - The fixed-point implementation is not working correctly on MSVC 2017
    or later, when built with optimization enabled (Release mode, 64-bit).
    On this compiler/platform, optimization needs to be disabled when using the
    fixed-point library (for all SystemC versions before and including 2.3.2).

  - Some paths in this release are longer than the historical 99 character
    limit of tar archives, and several Windows archivers (e.g. WinZip)
    have been reported to trip over this.  The open source archiver 7-zip
    (http://7-zip.org) is known to work.

  - Some warnings related to overloaded virtual functions are reported when using 
    gcc 13 or clang17. They will be fixed in an upcoming version of the library. 

## 9. Fixed-point library

SystemC contains a fixed-point data-types package.

Compile-time macro `SC_INCLUDE_FX` must be defined in order to build
applications that use fixed point types. You can specify a compiler
flag, e.g., `g++ -DSC_INCLUDE_FX` or use a define statement before
you include `systemc.h`, e.g.:

```
  #define SC_INCLUDE_FX
  #include "systemc.h"
```
