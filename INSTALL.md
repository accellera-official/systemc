# Installation Notes for SystemC

**Contents**
 * [Installation Notes for Unix][unix]
 * [Installation Notes for Windows][win]
 * [SystemC Library Configuration Switches][config]

> **Note**  
>  A CMake-based build system for SystemC is part of this distribution,
>  which unifies the configuration of the SystemC sources on different Unix
>  and Windows platforms.  It is able to generate the necessary files to
>  compile/install SystemC using different command-line build tools (e.g.,
>  GNU Make or Ninja) and IDEs (e.g., Eclipse, Xcode, MS Visual Studio C++,
>  or Kdevelop).  The installation notes for SystemC using CMake can be
>  found in the file [cmake/INSTALL_USING_CMAKE.md][cmake].

# Installation Notes for Unix

## System Requirements

SystemC can be installed on the following UNIX, or UNIX-like platforms:

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

Note: _Not all combinations are equally well-tested and some combinations
      may not work as expected.  Please report your findings by following
      the instructions in the [CONTRIBUTING](CONTRIBUTING.md) file._

The [RELEASENOTES.md](RELEASENOTES.md) file contains a list of detailed platforms,
architectures, and compiler versions that have been used for testing this release.


## Sources for Compilers and Related Tools

To build, install, and use SystemC on UNIX platforms, you need
the following tools:

  1. GNU C++ compiler (version 9.3 or later), or
     Clang C++ compiler (version 13.0 or later)
  2. GNU Make (make)

GCC, Clang, and make are free software that you can
obtain from the following sources:

  * GCC     http://www.gnu.org/software/gcc/gcc.html
  * Clang   http://clang.llvm.org/
  * make    http://www.gnu.org/software/make/make.html


## Basic SystemC Installation

To install SystemC on a UNIX system, do the following steps:

  1. Change to the top level directory (e.g., `systemc-3.0.0`)

  2. Choose your compiler by setting the `CXX` environment variable
     (the configure script tries to guess the default compiler, if
      this step is omitted).
     If you use a POSIX-compatible shell (e.g., bash):
     ```bash
     export CXX="<compiler>"
     ```
     e.g. for GCC compilers
     ```bash
     export CXX=g++
     ```
     The Clang compiler is usually named `clang++`, thus e.g.
     ```bash
     export CXX=clang++
     ```
     When using a C shell (e.g., `csh`/`tcsh`), the syntax to set the
     environment variable is different:
     ```sh
     setenv CXX g++
     ```
     You can also specify an absolute path to the compiler of your choice.
     See also the Section [Compilation and Linking Options][comp] below.

  3. Generate the configuration executable (only for git clone)

     Note: _This step is required when using a git clone.  It requires the 
            GNU Autotools `libtoolize`, `aclocal`, `automake`, and `autoconf`.
            A SystemC release already contains the configuration executable so
            this step can be ignored._

     ```bash
     ./config/bootstrap
     ```

  4. Create a temporary directory, e.g.,
     ```bash
     mkdir objdir
     ```

  5. Change to the temporary directory, e.g.,
     ```bash
     cd objdir
     ```

  6. **Configure the package** for your system, e.g.,
     (The `configure` script is explained below.)

     ```bash
     ../configure
     ```

     While the 'configure' script is running, which takes a few moments,
     it prints messages to inform you of the features it is checking.
     It also detects the platform.

     _Note for System V users_:  
     If you are using 'csh' on an older version of System V, you might
     need to use the `sh ../configure` command instead of `../configure`.
     Otherwise, 'csh' will attempt to 'configure' itself.

     Note: _As IEEE Std. 1666-2023 mandates C++17 as the baseline for SystemC
            implementations, make sure you enable the compiler flag to 
            select C++17, e.g.:

     ```bash
     ../configure 'CXXFLAGS=-std=c++17'
     ```

     SystemC 3.0.0 includes a fixed-point package that is always built.
     When compiling your applications with fixed-point types, you still have
     to use compiler flag `-DSC_INCLUDE_FX`.  Note that compile times might
     increase when using this compiler flag.

     In case you want to install the package in another place than the
     top level directory, configure the package e.g. as follows:

     ```bash
     ../configure --prefix=/usr/local/systemc-3.0.0
     ```

     Note: _Make sure you have created the target directory before installing
     the package.  Do _not_ use `/usr/local` as a prefix, unless you
     follow the Unix/FHS directory layouts (see below)._

     A fine grained configuration of the installation directories can
     be achieved via additional options, given to the configure script.

     By default, the files are installed directly to the `PREFIX` directory
     root and the library is installed to `PREFIX/lib-<TARGETARCH>`,
     depending on the current target architecture.  This may be undesired
     in cases where the package is meant to be installed in a system-wide
     location as part of shared (default) library and include hierarchies
     (e.g. `/usr/local`, `/usr`, `/opt`, ...).  To follow the Unix/FHS
     directory standards, you can use the following options:

     ```
       --with-unix-layout     use Unix directory layout for installation
                              [default=no]
     ``` 
     when "yes", the following (fine-grained) settings will be used:
     ```
       --includedir=DIR       C++ header files      [PREFIX/include]
       --libdir=DIR           object code libraries [EPREFIX/lib]
       --docdir=DIR           documentation root    [DATAROOTDIR/doc/systemc]
     ```

      The library destination itself can be further and separately configured
      by using the following option:

      ```
        --with-arch-suffix    add suffix to library installation directory
                              [default=-<TARGETARCH>]
      ```

      With this option, one can easily follow e.g. the "multi-arch"
      conventions on some platforms:

      ```bash
      ../configure --with-arch-suffix=32                # lib32
      ../configure --with-arch-suffix=/x86_64-linux-gnu # lib/x86_64-linux-gnu
      ```
 
     Several options are available to the configure script to modify
     the compiler configuration and the selection of certain features:

     ```
       --disable-shared       do not build shared library (libsystemc.so)
       --enable-debug         include debugging symbols
       --disable-optimize     disable compiler optimization
       --enable-pthreads      use POSIX threads for SystemC processes
     ```

     See the section on the general usage of the `configure` script and
     `../configure --help` for more information.

     Note: _When linking against a static library of SystemC with asynchronous
            update support enabled, you may need to explicitly link against the
            pthread library as well (`-lpthread`)._

     Note: _If you change the configuration after having compiled the
            package already, you should run a `make clean` before
            recompiling._

  7. **Compile the package**

     ```bash
     make
     ```

     Note: _The explicit make targets `opt` and `debug`, etc. have
            been removed in this package.  Use the corresponding
            options to the configure script instead._

  8. At this point you may wish to verify the compiled package by
     testing the example suite.

     ```bash
     make check
     ```

     This will compile and run the examples in the subdirectory
     examples.

  9. **Install the package**

     ```bash
     make install
     ```

  10. You can now remove the temporary directory, .e.g,

      ```bash
      cd ..
      rm -rf objdir
      ```

      Alternatively, you can keep the temporary directory to allow you to
      1) Experiment with the examples.
      2) Later uninstall the package.
     
      To clean up the temporary directory, enter:
      ```bash
      make clean
      ```

      To uninstall the package, enter:
      ```bash
      make uninstall
      ```

### Running the Examples

Copies of the examples reside in the temporary directory - see
instruction 7 above for details on building and running them.

In addition, a copy of the example code resides in the directory
examples at the highest level of the installation (or in the
shared documentation install directory).

Use the Makefiles provided in  the `examples` directory as templates
for Makefiles you need for compiling your own examples.


### Using the Configure Script

The `configure` shell script tries to determine the correct values for
various system-dependent variables used during compilation.  It uses
these values to create a `Makefile` in each directory of the package.
It also creates one or more `.h` files containing system-dependent
definitions if needed.  Then, it creates the following files:

* `config.status`  
    A shell script that you can run at another time to
    recreate the current configuration.

* `config.cache`  
    A file in which the configure test results are
    saved to speed up reconfiguration.
    Data is appended to the `config.cache` file.
    You can remove unwanted data.

* `config.log`  
    A file in which compiler output is saved.
    This is used to debug the configure script.

If you need to use other commands to successfully compile the package
on your system, please try to determine if the configure script can be used
for these commands.  Then, send either a diff file or instructions about
the commands you used to the e-mail address provided in the README file.
This information will be used to improve the installation process in
the next release.

The `configure.ac` file is provided in case you want to change or regenerate
the `configure` script, for example to use a newer version of `autoconf`.
The `configure.ac` file is used by the `autoconf` program to create the
`configure` script.

Note for (key) developers:

  In case you have changed the `configure.ac` file or one of the
  `Makefile.am` files:

  - Use 
    ```bash
    make distclean
    ```
    to remove the generated `configure` script, the generated `aclocal.m4`
   file and the generated `Makefile.in` files.

  - Use the `config/bootstrap` script to generate the `configure` script
    and the necessary `Makefile.in` files.  This script makes use of the
    GNU auto-tools `aclocal`, `automake`, and `autoconf`.


## Compilation and Linking Options

Some systems require compilation or linking options that the `configure`
script does not define.  You can define the initial values for these
options by setting them in your environment before running the
`configure` script.

Instead of passing the variables via the environment, it is preferred
to pass the values as options to the configure script, e.g.,:

```sh
../configure CXX=g++-9.3 LIBS=-lposix
```


### Specifying the System Type

Some features cannot be automatically determined by `configure` unless
it can detect the host type on which the package will run.
If it prints a message that it cannot determine the host type,
use the `--host=TYPE` option to define it.  `TYPE` can either be a
short system name, such as `sun4`, or a canonical name with three fields:

     CPU-COMPANY-SYSTEM

See the `config.sub` file for details about the values of each field. If
the `config.sub` file is not included in the package, the package does not
need to know the host type.

If you are building compiler tools for cross-compiling, you can also
use the `--target=TYPE` option to select the type of system for which
the code is produced and the `--build=TYPE` option to select the type of
system on which you are compiling the package.


### Sharing Defaults

You can set the default values that `configure` scripts share by
creating a site shell script called `config.site`. This file contains the
default values for variables like `CC`, `cache_file`, and `prefix`.
The `configure` script looks for the `config.site` file in the following
search precedence:

  1. `PREFIX/share/config.site`
  2. `PREFIX/etc/config.site`

Alternatively, you can set the `CONFIG_SITE` environment variable to the
site script path.

Note: _The `configure` script for some systems does not look for a site script._


### Operation Controls

The `configure` script recognizes the following additional options to control
its operation:

`--cache-file=FILE`  
        Use and save the test results in FILE instead of
        `./config.cache`.  Set FILE to `/dev/null` to disable caching
        when debugging `configure`.

`--help`  
        Print a summary of `configure` options and exit.

`--quiet`  
`--silent`  
`-q`  
        Do not print messages about checks being made.
        To suppress all normal output, redirect it to `/dev/null`.
        Error messages continue to print.

`--srcdir=DIR`  
        Look for the package's source code in directory DIR.
        Typically `configure` determines the directory automatically.

`--version`  
        Print the version of `autoconf` used to generate the `configure`
        script and exit.

Other options that are rarely used are available in the `configure` script.
Use the `--help` option to print a list.


---
# Installation Notes for Windows

This release has been tested on Visual C++ version 2019 running Windows 10.

Note: _This section covers the installation based on Microsoft Visual C++.
      For Cygwin or MinGW-based installations, see Section 1._


Note: _If you experience spurious errors about missing files in the
      downloaded archive, please make sure to either download the
      ZIP archive from accellera.org or use a reliable archive software,
      fully supporting modern tar archive versions._

 Some paths in the SystemC archive are longer than the historical
      99 character limit, and several Windows archivers (e.g. WinZip)
      have been reported to trip over this.  The open source archiver
      [7-zip](http://7-zip.org/) is known to work.



## Microsoft Visual C++ 2017 (compiler version 15.0) or later

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
set to compile for Visual C++ 2019.  Select 'Build <example>.exe' under the
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


## Creating SystemC Applications

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

3. Select the 'New' icon and browse to: C:\systemc-3.0.0\msvc16\systemc\debug

4. Select show directories for: Include files

5. Select the 'New' icon and browse to: C:\systemc-3.0.0\src

To add the include file and library directory search paths for the current
project only:

1. Display the project Property Pages by selecting 'Properties...' from
   the Project menu.

2. From the C/C++ tab, select the General properties and type the path to the
   SystemC 'src' directory in the text entry field labeled
  'Additional include directories' (e.g., the examples use `..\..\..\src`).

3. From the Linker tab, select the General properties and type the path to
   the SystemC library:   ...\systemc-3.0.0\msvc16\systemc\debug
   in the 'Additional Library Directories:' box.

9. Click OK.


## Building against a SystemC DLL

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


---
# SystemC Library Configuration Switches

In addition to the explicitly selectable feature given as options to
the `configure` script (see 1.), some aspects of the library
implementation can be controlled via

 - preprocessor switches given during library build
 - preprocessor switches added while building a SystemC application
 - environment variables

The currently supported switches are documented in this section.

## Preprocessor switches

Additional preprocessor switches for the library build can be passed
to the configure script via the `CXXFLAGS` variable:

```sh
  ../configure CXXFLAGS="-DSC_OVERRIDE_DEFAULT_STACK_SIZE=0x80000"
```

In Visual C++, the preprocessor symbols can be added to the project
configuration via the 'C/C++' tab under the 'Preprocessor' properties
in the 'Preprocessor definitions' setting.  Alternatively, you can add
the switches to the `SystemC.vsprops` property sheet to apply these
settings to all build configurations.


 * `SC_CPLUSPLUS`  
   Override automatically detected C++ standard support

   This setting allows downgrading the assumed version of the
   underlying C++ standard on the current platform.  By default,
   the latest supported version is chosen.
   Supported values are
     * `SC_CPLUSPLUS=201703L` (C++17, ISO/IEC 14882:2017)
     * `SC_CPLUSPLUS=202002L` (C++20, ISO/IEC 14882:2020)

   Note: _This symbol needs to be consistently defined in the library
            and any application linking against the built library._


 * `SC_DEFAULT_WRITER_POLICY=<sc_writer_policy>`  
   Override default value for the signal writer policy

   This setting allows deactivating the multiple writer checks for
   sc_signals at (application) compile time.  This mechanism supersedes
   the old environment variable `SC_SIGNAL_WRITE_CHECK` (see below).

   Supported values:
     * `SC_ONE_WRITER`        (default)
     * `SC_MANY_WRITERS`      (allow multiple writers in different deltas)
     * `SC_UNCHECKED_WRITERS` (non-standard, disable all checks)

   Note: _Only effective when building an application._  
   Note: _This setting needs to be consistently set across all
         translation units of an application._


 * `SC_DISABLE_VCD_SCOPES`  
   Disable grouping of VCD trace variables in hierarchical scopes
   by default

   Note: _Only effective during library build._  
   See : Environment variable `SC_VCD_SCOPES`


 * `SC_DISABLE_COPYRIGHT_MESSAGE`  
   Do not print the copyright message when starting the application

   Note 1: _This does not remove the copyright from the binary.
           sc_core::sc_copyright() still works as expected._  

   Note 2: _Only effective during library build._  
   See : Environment variable `SC_COPYRIGHT_MESSAGE`


 * `SC_ENABLE_ASSERTIONS`  
   Always enable the `sc_assert` expressions

   Some build systems define `NDEBUG` by default in optimised build
   configurations.  As a result, the SystemC assertion macro `sc_assert()`
   is disabled (similar to the C `assert()` macro).  By defining this
   preprocessor symbol (when building the library and/or an application),
   the `sc_assert()` checks are always enabled, irrespectively of the
   definition of `NDEBUG`.


 * `SC_INCLUDE_FX`  
   Enable SystemC fixed-point data-types

   To improve compilation times, the fixed-point data-types are not enabled
   by default in an SystemC application.
   Define this symbol before including the SystemC header in your
   application, if you want to use the SystemC fixed-point types.

   Note: _Is by default always defined during the library build to enable
         later use of the fixed-point data-types in an application._

   Note: _Can be optionally set per translation unit in an application._


 * `SC_INCLUDE_WINDOWS_H`  
   Explicitly include `<windows.h>` header from `<systemc>` header

   Previous versions of SystemC always included the full `<windows.h>`
   header on all Windows platforms.  This adds unnecessary bloat to
   many SystemC applications, increasing compilation times.
   If you rely on the inclusion of the `<windows.h>` header in your
   application, you can add this symbol to the list of preprocessor
   switches for your compiler.

   Note: _Only effective when building an application._


 * `SC_INCLUDE_EXTRA_STD_HEADERS`  
   Include `<cstring>` and `<sstream>` headers from `<systemc>` header

   Previous versions of SystemC implicitly included the `<cstring>` and
   `<sstream>` headers on all platforms without depending on their
   contents.
   If you rely on the inclusion of these headers in your application,
   you can add this symbol to the list of preprocessor switches for
   your compiler.

   Note: _Only effective when building an application._


 * `SC_ALLOW_MACROS_WITHOUT_SEMICOLON`  
   Allow using (process) macros without terminating semicolon

   Previous versions of SystemC allowed using some macros
   without a trailing semicolon.  This is no longer supported by
   default.  Defining the above macro restores the old behavior.
   Affected macros: `SC_METHOD`, `SC_(C)THREAD`.

   Note: _Only effective when building an application._


 * `SC_OVERRIDE_DEFAULT_STACK_SIZE=<size>`  
   Define the default stack size used for SystemC (thread) processes

   Note: _Only effective during library build._


 * `SC_USE_STD_STRING`
   Define `sc_string` symbol as an alias to `std::string`

   Pre-IEEE-1666 versions of SystemC included an `sc_string` class for
   string objects.  This class has been superseeded by `std::string` these
   days.


 * `SC_WIN_DLL`  
   Build (against) a DLL build of SystemC (Windows/MSVC only)

   Needs to be set when building a SystemC DLL on Windows, as well
   as when building an application/library to be linked against a
   DLL version of SystemC.


## Influential environment variables

Currently, three environment variables are checked at library load time
and influence the SystemC library's behavior:

 * `SC_COPYRIGHT_MESSAGE=DISABLE`  
    Run-time alternative to `SC_DISABLE_COPYRIGHT_MESSAGE` (see above).

 * `SC_SIGNAL_WRITE_CHECK=DISABLE`, `SC_SIGNAL_WRITE_CHECK=CONFLICT`  
    Run-time alternative to `SC_DEFAULT_WRITER_POLICY` (see above).

   - `DISABLE`  = disable all checks for conflicting writes
                  (`SC_UNCHECKED_WRITERS`)
   - `CONFLICT` = detect conflicting writes within the same
                  evaluation phase (`SC_MANY_WRITERS`)

 * `SC_DEPRECATION_WARNINGS=DISABLE`  
    Do not issue warnings about using deprecated features as of
    IEEE Std. 1666-2011.

 * `SC_VCD_SCOPES=DISABLE`, `SC_VCD_SCOPES=ENABLE`  
    Run-time configuration of hierarchically scoped names in VCD
    trace files (see `SC_DISABLE_VCD_SCOPES`).


Usually, it is not recommended to use any of these variables in new or
on-going projects.  They have been added to simplify the transition of
legacy code.


[unix]: #installation-notes-for-unix
[comp]: #compilation-and-linking-options
[win]: #installation-notes-for-windows
[config]: #systemc-library-configuration-switches
[cmake]: cmake/INSTALL_USING_CMAKE.md
