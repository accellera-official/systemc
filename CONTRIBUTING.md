How to Contribute
=================

 This repository is owned by the [Accellera Systems Initiative][1] and
 is maintained by the [SystemC Language Working Group][2] (LWG)
 according to the [Accellera Policies and Procedures][3].

 **Contributions to this reference implementation can only be
   accepted from Accellera members.**

### Join the Accellera SystemC Language Working Group

 If you would like to contribute to the development of SystemC and/or this
 reference implementation, have your company, organization, or university
 join Accellera and its working groups.
 Find out more information at http://www.accellera.org/about/join.
 If your company, organization or university is already an Accellera member,
 you can request to [join the SystemC Language Working Group here][4].

### Join the SystemC community

 If you are not an Accellera member, please join the **[SystemC community
 forum][5]** to provide feedback, report bugs and join the general
 discussion around the evolution of SystemC.

[1]: https://www.accellera.org
[2]: https://accellera.org/activities/working-groups/systemc-language
[3]: http://accellera.org/about/policies
[4]: http://workspace.accellera.org/apps/org/workgroup/lwg/
[5]: https://forums.accellera.org/forum/9-systemc/

---------------------------------------------------------------------
Issue reporting
---------------------------------------------------------------------

You can post the bugs and suggestions of general interest to the
[SystemC Community Forum][5].  When reporting bugs, please specify
the following information (if applicable):

  1. SystemC version
  2. platform, compiler, flags
  3. description of the problem
  4. steps to reproduce the problem
  5. compile/runtime warnings and errors
  6. code sample, not more than 100 lines to demonstrate the problem

> **Note**  
>  All bugs will only be tested against the latest publicly available
>  version of the product.

> **Note**  
>  All C++ compilers that SystemC supports have bugs of different
>  degree of severity. We cannot fix those bugs.
>  Please report them to the compiler vendor.

Accellera WG members have access to the WG-internal issue tracking
at GitHub, as described [here][6].

[6]: docs/DEVELOPMENT.md#issue-tracking

---------------------------------------------------------------------
Repository organization
---------------------------------------------------------------------

The central source code repository of the Accellera SystemC implementation is
hosted in two repositories at [GitHub](http://github.com).  The main
repositories are **private** to the [`OSCI-WG` organization][7] and can be
found at:

 * https://github.com/OSCI-WG/systemc             (core SystemC library)
 * https://github.com/OSCI-WG/systemc-regressions (regression test suite)

A read-only, **public** copy of these repositories can be found at

 * https://github.com/accellera/systemc             (core SystemC library)
 * https://github.com/accellera/systemc-regressions (regression test suite)

To obtain access to the repositories and the GitHub organization in general,
LWG members can contact the LWG chairs at <mailto:lwg-chair@lists.accellera.org>
including their GitHub account name.

[7]: https://github.com/osci-wg "Accellera WG GitHub organization"
