# Installation Notes for SystemC

## Table of Contents

1. System Requirements

2. Installation Notes for CMake

   2.1 Sources for Compilers and Related Tools

   2.2 Installation Steps

   2.3 Cross compilation

   2.4 Finding and Using SystemC with CMake

3. Installation Notes for Windows using Visual Studio C++

   3.1 Microsoft Visual Studio C++ 2019 (compiler version 16.0) or later

   3.2 Creating SystemC Applications

   3.3 Building against a SystemC DLL

4. Known Problems

5. Resources


## 1. System Requirements

SystemC can be installed on the following platforms:

  * Linux
    * Architectures
      - x86_64 (64-bit)
      - x86 (32-bit) application running on x86_64 (64-bit) kernel
        (`../configure --host=i686-linux-gnu`)
    * Compilers
      - GNU C++ compiler
      - Clang C++ compiler
      - or compatible

  * macOS
    * Architectures
      - Apple Silicon
      - x86_64 (64-bit)
    * Compilers
      - Clang C++ compiler
      - or compatible

  * Windows
    * Compatibility layer
      - MinGW / MSYS
    * Architectures
      - x86_64 (64-bit)
    * Compilers
      - GNU C++ compiler
      - or compatible
    * Visual Studio C++
      -  Win32 (32-bit)
      -  x64 (64-bit)
      -  Static library, DLL

Note 1: _IEEE Std. 1666-2023 mandates C++17 as the baseline for SystemC
        implementations, see [RELEASENOTES.md](RELEASENOTES.md).  Make
        sure to configure your compiler accordingly (see below)._

Note 2: _Not all combinations are equally well-tested and some combinations
        may not work as expected.  Please report your findings by following
        the instructions in the [CONTRIBUTING](CONTRIBUTING.md) file._

Note 3: _CMake is the recommended build flow SystemC. Installation using the
        legacy GNU Autotools build flow can be found 
        [here](docs/INSTALL_USING_AUTOTOOLS.md)._

## 2. Installation Notes for CMake

### 2.1 Sources for Compilers and Related Tools

To build, install, and use SystemC, you need the following tools:

  1. GNU C++ compiler (version 9.3 or later), or
     Clang C++ compiler (version 13.0 or later), or
     Visual Studio C++ (version msvc16 (2019) or later)
  2. GNU Make (make) or Ninja
  3. CMake

GCC, Clang, make, Ninja, and CMake are free software that you can
obtain from the following sources:

  * GCC     https://www.gnu.org/software/gcc/index.html
  * Clang   https://clang.llvm.org/
  * make    https://www.gnu.org/software/make/make.html
  * Ninja   https://ninja-build.org/
  * CMake   https://cmake.org/

The CMake build scripts are compatible with CMake >=3.5 and have been
tested on the following OS/processor/compiler platforms:

  * macOS >= 13.6 until 15.4 (Xcode/AppleClang): x86_64, Apple Silicon, universal binary
  * Linux (GCC, Clang): i386, x86_64
  * Linux (GCC): aarch64
  * Windows (MSYS2 with the MinGW-W32 or MinGW-W64 GCC toolchains, Visual C++):
    x86 (Win32), AMD64 (x64)

### 2.2 Installation Steps

Typically, the following steps need to be followed to compile the
sources using CMake:

  1. Download and install CMake from [this website][1]
     (Linux distributions provide often a package).

  2. Create a build subdirectory:

         $ pwd
         .../systemc/
         $ mkdir build

  3. Launch the interactive cmake console program `ccmake` (on Unix) or
     the CMake GUI (Unix, macOS, Windows) and specify the source
     (`.../systemc`) and build directory (`.../systemc/build`).  For
     the console based `ccmake`, this is done by changing into the build
     directory and launching `ccmake` with the relative path to the source
     root directory as an argument:

         $ cd build/
         $ ccmake ..

     For cmake-gui, the same approach works:

         $ cd build/
         $ cmake-gui ..

     Alternatively, the CMake GUI allows to directly enter the paths to the
     sources and build directory in the top of its main window.

  4. Configure the sources by pressing 'c' or clicking on
     "Configure".  Depending on the operating system, CMake will ask the user
     what it should generate Makefiles or IDE project files (Eclipse,
     Xcode, Kdevelop, Microsoft Visual Studio C++).  CMake will parse the file
     `CMakeLists.txt` in the source root directory and configure the sources
     accordingly carrying out all necessary platform-specific checks.

     The CMake build scripts detect the actual features provided by the
     target (Unix or Windows) platform.  On Unix (including macOS), shared
     libraries can be built.  If the QuickThreads library provides support
     for the target processor, it will be automatically used.  Otherwise, we
     rely on Pthreads on Unix and Fiber on Windows.  By default, the SystemC
     library installation will follow the GNU standard installation layout
     so that also other SystemC libraries (SystemC, SCV, TLM, AMS
     extensions) can be installed into the same directory hierarchy (Unix:
     `/opt/systemc/`; Windows: `$ENV{ProgramFiles}/SystemC/`).  The target
     platform's conventions are respected meaning usually `include/` for the
     headers and `lib/`, `lib64/`, or `lib/<multiarch-tuple>/` for the
     libraries.  The `lib-${SYSTEMC_TARGET_ARCH})/` convention is not used
     by default, as `${SYSTEMC_TARGET_ARCH}` does not reliably encode the
     OS/processor/compiler tuple.

     The build process can be configured through the configuration
     variables offered to the user in the CMake console and GUI (`ccmake` and
     `cmake-gui`, respectively).  Each variable is annotated with a brief
     documentation string.  The most important variables are:

     * `BUILD_SHARED_LIBS`
       Build shared instead of static libraries
       (default: `ON` if not targeting Windows).

     * `BUILD_SOURCE_DOCUMENTATION`
       Build source code documentation using Doxygen (default: `OFF`).

     * `ENABLE_EXAMPLES`
       Add build targets for all examples under the `examples/` folder,
       add composite targets `all-examples` and `check-examples` to
       build and run all of them (default: `ON`).

     * `ENABLE_REGRESSION`
       Add build targets for all regression tests under the `tests/` folder,
       add composite targets `all-tests` and `check-tests` to build and run
       all of them (default: `OFF`).

     * `CMAKE_BUILD_TYPE`
       Specifies the build type on single-configuration generators.
       (default: `Release`).

     * `CMAKE_CXX_STANDARD`
       C++ standard to build all targets.  (default: `17`).

     * `CMAKE_CXX_STANDARD_REQUIRED`
       The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.
       (default: `ON`).

     * `CMAKE_INSTALL_PREFIX`
       Root directory of the SystemC libraries installation (defaults to
       `$ENV{SYSTEMC_HOME}` if set to an absolute path and otherwise to either
       `/opt/systemc/` (Unix-like platforms including CYGWIN),
       `$ENV{ProgramFiles}/SystemC/` (on Windows systems), or
       `${CMAKE_INSTALL_PREFIX}/systemc`.

     * `CMAKE_OSX_ARCHITECTURES`
       Architectures for cross-compilation on macOS (default: _empty_,
       i.e., only for the system processor).

     * `CMAKE_VERBOSE_MAKEFILE`
       Generate a verbose Makefile (default: `OFF`).

     * `ENABLE_WARNINGS_AS_ERRORS`
       Treat compiler warnings as errors on supported compilers (default: `OFF`).

     * `DISABLE_COPYRIGHT_MESSAGE`
        Do not print the copyright message when starting the application.
        (default: `OFF`).

     * `ENABLE_ASSERTIONS`
       Always enable the `sc_assert` expressions (default: `ON`).

     * `ENABLE_PTHREADS`
       Use POSIX threads for SystemC processes instead of QuickThreads on Unix
       or Fiber on Windows.

     * `SystemC_TARGET_ARCH`
       Target architecture according to the Accellera SystemC conventions set
       either from `$ENV{SYSTEMC_TARGET_ARCH}`, `$ENV{SYSTEMC_ARCH}`, or
       detected by CMake.

     * `INSTALL_TO_LIB_BUILD_TYPE_DIR`
       Install the libraries to `lib-${CMAKE_BUILD_TYPE}` to enable parallel
       installation of the different build variants. (default: `OFF`).

     * `INSTALL_TO_LIB_TARGET_ARCH_DIR`
       Install the libraries to `lib-<target-arch>` to facilitate linking
       applications, which build systems assume to find SystemC in
       `lib-<target-arch>`. (default: `OFF`).

     * `INSTALL_LIB_TARGET_ARCH_SYMLINK`
       On Unix, install a symlink `lib-<target-arch>` to
       `lib-${CMAKE_BUILD_TYPE}` facilitating the linking of user code, which
       build system assumes to find the SystemC libraries under
       `lib-<target-arch>`.  (default: `OFF`).

     Other configuration variables will be proposed by CMake depending on the
     OS and detected processor, compiler, and libraries.  For more information,
     please refer to the [CMake documentation][1].

     The configuration step needs to be repeated until no new configuration
     variables appear.

  5. Generate the Makefiles or IDE project files by pressing 'g' or
     clicking on "Generate".

  6. Compile, test, and install the libraries using make or the IDE:

         $ make
         $ make check
         $ sudo make install

More documentation on using CMake is available from the the [CMake Wiki][3].

### 2.3 Cross Compilation

CMake supports cross compilation on a host system for a different
target architecture.  Depending on the scenario, different approaches
can be used.

#### Cross Compilation for 32-bit on a 64-bit platform using GCC

This section is primarily relevant for users of 64-bit versions of
Linux and Windows.

On 64-bit platforms like Linux x86_64 and Windows AMD64, the OS
provides usually for backward-compatibility the capability to execute
64-bit and 32-bit applications.  To this end, it provides 32-bit and
64-bit versions of all runtime libraries (on Linux, they can be
usually found in the lib/ and lib64/ directories).  On these platforms,
the GCC toolchain is usually installed in a multi-lib variant, which
supports the compilation and linking of 64-bit and 32-bit applications
and libraries.  The word width of the target architecture is simply
chosen with the `-m64` or `-m32` option, which has to be consistently
passed at each compiler and linker call.

Therefore, it is in principle sufficient to add `-m32` to the
corresponding CMake variables containing the flags to be passed to the
linker and compiler:

       CMAKE_C_FLAGS
       CMAKE_CXX_FLAGS
       CMAKE_EXE_LINKER_FLAGS
       CMAKE_MODULE_LINKER_FLAGS
       CMAKE_SHARED_LINKER_FLAGS
       CMAKE_STATIC_LINKER_FLAGS

These flags should be already set upon the first CMake invocation in a
new build directory, as most system introspection results are cached
over multiple CMake runs.  You can do so via the `-D<VAR>=<VALUE>`
option of `cmake`, `ccmake`, and `cmake-gui`.

In a similar way, the particular C or C++ compiler can be chosen by
providing the name (optionally preceded by the path) of the compiler
executables:

       CMAKE_C_COMPILER
       CMAKE_CXX_COMPILER

However, it is very handy to know that CMake honors the standard
environment variables `CC` (C compiler), `CXX` (C++ compiler), `CFLAGS`
(C compiler flags), `CXXFLAGS` (C++ compiler flags), and `LDFLAGS` (linker
flags), which are also used by other tools such as GNU Make, to
initialize the above variables upon its first call in a new build
directory. Note, that `CPPFLAGS` is currently ignored by CMake, see
[this issue][4].

On certain platforms, CMake might have trouble to find the right
library for the target processor architecture when using
`find_package()` or `find_library()` in the CMake scripts.  In that case, it
can help to explicitly set the following global property in your CMake
file before issuing any `find_package()` or `find_library()` in your CMake
script:

       set_property (GLOBAL FIND_LIBRARY_USE_LIB64_PATHS <TRUE|FALSE>)

If set to `TRUE`, it will look for the library in `lib64/` instead of `lib/`.
This property is usually automatically set on platforms known to need
it, but depending on the chosen compiler options, it may need adjustment.

#### Cross Compilation on macOS

On macOS, a similar approach as in
[Section 3.1](#31-microsoft-visual-studio-c-2019-compiler-version-160-or-later)
can be adopted.  However,
it has to be noted that Apple's versions of GCC and Clang don't support
`-m32` and `-m64`, but the more flexible `-arch <arm64|x86_64>`
parameter.  The latter can be even repeated several times on the command
line to create universal binaries able to run on several processor
architectures.  CMake has native support for this command line option,
which value can be set centrally in the CMake scripts and will be then
passed on to the compilers and linker.
It is sufficient to set the variable `CMAKE_OSX_ARCHITECTURES` to the
desired target architecture(s).  Multiple architectures are separated
with a `;`. You can do so already on the command line

      $ cmake .. -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64'

or in variable editors of `ccmake` and `cmake-gui`.

If you are using MacPorts or Homebrew versions of GCC, you will have
to make sure that you install universal variants of the compiler
toolchain.  Then, you can follow the instruction in
[Section 3.1](#31-microsoft-visual-studio-c-2019-compiler-version-160-or-later).

#### Generic Cross Compilation to a Target System

As CMake cannot guess the target system, a number of variables have to
be preset so that the correct compiler toolchain and libraries are
found.  It is recommended to do this with a so-called toolchain file,
which is passed as an argument to `cmake`, `ccmake` or `cmake-gui` using the
option `-DCMAKE_TOOLCHAIN_FILE=<path-to-file>`. The file in CMake
syntax has to define the following variables:

 * `CMAKE_SYSTEM_NAME` (mandatory), e.g., "Linux", "Windows", "Darwin"
 * `CMAKE_SYSTEM_VERSION` (optional)`
 * `CMAKE_SYSTEM_PROCESSOR` (optional), e.g., "i386", "x86_64"
 * `CMAKE_C_COMPILER` (mandatory), name of C compiler executable
 * `CMAKE_CXX_COMPILER` (mandatory), name of C compiler executable
 * `CMAKE_FIND_ROOT_PATH` (optional), root to target include and library directories
 * `CMAKE_FIND_ROOT_PATH_MODE_PROGRAM`, either "NEVER", "ONLY", or "BOTH" (default)
 * `CMAKE_FIND_ROOT_PATH_MODE_LIBRARY`, either "NEVER", "ONLY", or "BOTH" (default)
 * `CMAKE_FIND_ROOT_PATH_MODE_INCLUDE`, either "NEVER", "ONLY", or "BOTH" (default)

The necessary steps are discussed in detail on the CMake [Cross Compiling][5] page.

### 2.4 Finding and Using SystemC with CMake

The CMake build scripts install CMake package config files to facilitate finding
the SystemC and TLM libraries.  The packages are called `SystemCLanguage` and
`SystemCTLM`.  As the TLM support is compiled into the SystemC library, the package
`SystemCTLM` just finds the matching SystemC library as a dependency.  As SystemC
is usually installed in its own directory tree, you have to tell CMake where to
look for the package config files by setting the variable `CMAKE_PREFIX_PATH` to
the root directory of your SystemC installation before calling `find_package()`.

The `SystemCLanguage` package provides the imported target `SystemC::systemc` to
which an application or library should link to.  Linking to an imported target
ensures that all necessary preprocessor definitions, compiler options, and
transitive link libraries are used when compiling and linking the application.

The `SystemCLanguage` package exports a number of variables:

 * `SystemC_TARGET_ARCH`
   Target architecture according to the Accellera SystemC conventions

 * `SystemC_CXX_STANDARD`
   Preferred C++ standard

 * `SystemC_CXX_STANDARD_REQUIRED`
   Determine whether the selected C++ standard is a requirement

The `SystemC_CXX_STANDARD` and `SystemC_CXX_STANDARD_REQUIRED` variables should be
used to initialize the respective `CMAKE_CXX_STANDARD` and
`CMAKE_CXX_STANDARD_REQUIRED` variables in the CMake build scripts of the
application/library. Alternatively, the `CXX_STANDARD` and `CXX_STANDARD_REQUIRED`
target properties can be set directly.

As an example, here is a minimal `CMakeLists.txt` to compile the `simple_perf`
SystemC example as a stand-alone application:

      --- Start: CMakeLists.txt ---
      cmake_minimum_required(VERSION 3.16)
      project(simple_perf CXX)

      set (CMAKE_PREFIX_PATH /opt/systemc)
      find_package(SystemCLanguage CONFIG REQUIRED)

      set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
           "C++ standard to build all targets. Minimum version is C++17.")
      set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
           "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.")

      add_executable(simple_perf simple_perf.cpp)
      target_link_libraries(simple_perf SystemC::systemc)
      --- End: CMakeLists.txt ---


## 3. Installation Notes for Windows using Visual Studio C++

This release has been tested on Visual C++ version 2019 running Windows 10.

Note: _This section covers the installation based on Microsoft Visual C++.
      For Cygwin or MinGW-based installations, see
      [Section 2](#2-installation-notes-for-cmake)._

Note: _If you experience spurious errors about missing files in the
      downloaded archive, please make sure to download the release
      from the [Accellera public SystemC repository][9] and use an
      extractor supporting tar.gz archives._

### 3.1 Microsoft Visual Studio C++ 2019 (compiler version 16.0) or later

The download directory contains two subdirectories: `msvc16` and
`examples/build-msvc`.

The `msvc16` directory contains the project and workspace files to
compile the `systemc.lib` library. Double-click on the `SystemC.sln`
file to launch Visual C++ 2019 with the workspace file.  The workspace file
will have the proper switches set to compile for Visual C++ 2019.
Select `Build SystemC` under the Build menu or press F7 to build
`systemc.lib`.

The `examples/build-msvc` directory contains the project and workspace
files to compile the SystemC examples.  Go to one of the examples
subdirectories and double-click on the `.vcxproj` file to launch Visual C++
with the workspace file.  The workspace file will have the proper switches
set to compile for Visual C++ 2019.  Select `Build <example>.exe` under the
Build menu or press F7 to build the example executable.

For convenience, a combined solution file `SystemC_examples.sln` with
all example projects can be found in the `examples/build-msvc` directory.
A similar solution file `tlm_examples.sln` for the TLM examples is available
as well.

The provided project files are prepared for both the 32-bit (Win32) and
64-bit (x64) configurations.  Please refer to the Microsoft Visual Studio
documentation for details about 64-bit builds.

In addition to building static libraries for SystemC, the provided project
files include support for building a SystemC DLL (configurations `DebugDLL`,
`ReleaseDLL`).

### 3.2 Creating SystemC Applications

 1. Start Visual Studio.  From the Start Page select New Project and Win32
    Console Project.  Type the project name and select a suitable location
    then click OK.

 2. Select the Application Settings page of the Win32 Application Wizard
    and make sure the 'Empty project' box is ticked.  Click 'Finish' to
    complete the wizard.

 3. Add new/existing C++ files to the project and edit code.

 4. Display the project Property Pages by selecting 'Properties...' from
    the Project menu.

 5. From the C/C++ tab, select the Language properties and set
    'Enable Run-Time Type Info' to Yes.

 6. From the C/C++ tab, select the Command Line properties and add `/vmg`
    to the 'Additional Options:' box.

 7. From the Linker tab, select the Input properties and type `systemc.lib`
    in the 'Additional Dependencies' box.

 8. Click OK.

Also make sure that the compiler and linker can find the SystemC header
and library files respectively.  There are two ways to do this:

To update the include file and library directory search paths for all
projects:

 1. Select Tools -> Options... and the Projects -> VC++ Directories tab

 2. Select show directories for: Library files

 3. Select the 'New' icon and browse to: `C:\systemc-3.0.0\msvc16\systemc\debug`

 4. Select show directories for: Include files

 5. Select the 'New' icon and browse to: `C:\systemc-3.0.0\src`

 To add the include file and library directory search paths for the current
 project only:

 1. Display the project Property Pages by selecting 'Properties...' from
    the Project menu.

 2. From the C/C++ tab, select the General properties and type the path to the
    SystemC `src` directory in the text entry field labeled
    'Additional include directories' (e.g., the examples use `..\..\..\src`).

 3. From the Linker tab, select the General properties and type the path to
    the SystemC library: `...\systemc-3.0.0\msvc16\systemc\debug`
    in the 'Additional Library Directories:' box.

 4. Click OK.

### 3.3 Building against a SystemC DLL

In order to link your application against a DLL-build of SystemC (build
configurations `DebugDLL`, `ReleaseDLL` in the SystemC library build),
several changes are needed.

1. Adjust the linker library directory settings to reference `DebugDLL`
   (or `ReleaseDLL`) instead of `Debug` or `Release`, respectively:
      `...\systemc-3.0.0\msvc16\systemc\DebugDLL`

2. Add the preprocessor switch `SC_WIN_DLL` to your project's properties
   (C/C++ -> Preprocessor -> Preprocessor Definitions).

3. When running the simulation, you need to add the location of the
   SystemC DLL to your `PATH` variable.


## 4. Known Problems

  - The CMake build scripts require more thorough testing of the various build
    configurations on as many platforms as possible.  For the moment, testing
    has been primarily done on macOS (x86_64 and i686), Linux (x86_64 and
    i686) and Windows 10 using Visual Studio 2019 (16) (Win32 and x64).

## 5. Resources

* [CMake][1]
* [CMake Documentation][2]
* [CMake Wiki][3]
* [Mastering CMake][6]
* [The MacPorts Project Homepage][7]
* [Homebrew - The missing package manager for macOS][8]


[1]: https://cmake.org/ "Cross Platform Make Homepage"
[2]: https://cmake.org/documentation/
[3]: https://gitlab.kitware.com/cmake/community/-/wikis/Home
[4]: https://gitlab.kitware.com/cmake/cmake/-/issues/12928
[5]: https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling
[6]: https://cmake.org/cmake/help/book/mastering-cmake/
[7]: https://www.macports.org/
[8]: https://brew.sh/
[9]: https://github.com/accellera-official/systemc
