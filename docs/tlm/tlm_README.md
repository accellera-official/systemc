# README for TLM-2.0

This is the proof of concept implementation for the TLM-2.0 standard,
as defined by the IEEE 1666-2023 SystemC Language Reference Manual.

# IMPORTANT

This version of the TLM-2.0 code contains updates compared to the previous version. For details of changes, please refer to the IEEE 1666-2023 SystemC Language Reference Manual.

# Licensing and Copyright

See the [LICENSE](../../LICENSE) file for SystemC.

# Installation

Please follow the [INSTALL.md](../../INSTALL.md) instructions for SystemC.

# Release Notes

See the separate SystemC [RELEASENOTES.md](../../RELEASENOTES.md) file.

# User Documentation

You can find documentation for this release on www.systemc.org
A doxygen.in configuration file is available which may be used to build
documentation using Doxygen [www.doxygen.org](https://www.doxygen.org).

The TLM library is documented in the IEEE 1666-2023 SystemC Language
Reference Manual.  This may be obtained by visiting the Accellera Systems
Initiative at [www.accellera.org](https://www.accellera.org).

# Directory Structure

SystemC and TLM code now have a common directory structure. The TLM header
files are in the same include directory as the SystemC header files.

The previous `unit_test` directory has been merged into the separate
systemc-regressions download, available from [www.accellera.org](https://www.accellera.org).

There are some examples in the folder `examples/tlm`, and documentation in 
`docs/tlm`.

# Build Support

Support for building the examples is provided for `unix`, and
`msvc` (project and make files).

Makefiles work hierarchically.  `build` is the default target; `clean`,
`run` (executes built `tests/examples`) and `check` (against expected 
results) targets are also provided.

On Unix/Linux/macOS, the `$SYSTEMC_HOME` environment variable should be set to
point to the SystemC root location; `$TARGET_ARCH` (or `$ARCH_SUFFIX`) should be
defined such that `$SYSTEMC_HOME/lib-$TARGET_ARCH` (or `lib$ARCH_SUFFIX`) is a
directory containing the SystemC library.

On Windows, the `SYSTEMC` environment variable should be used.

# Examples

There is a set of examples available under the `examples/tlm` directory, they
illustrate the key features of the TLM-2.0 standard. For more information see
the README.txt.

# Problems

Any problems related to the use of this proof-of-concept code can be
submitted to the Accellera SystemC TLM community forums at
http://forums.accellera.org/forum/14-systemc-tlm-transaction-level-modeling/
