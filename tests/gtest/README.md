# Summary

This directory contains a number of tests meant to ensure compliance with the
SystemC standard. All the test suites are written using gtest and linked against
SystemC with an `sc_main` that will run the tests.

## Writing New Tests

Identify a section or subsection in the IEEE1666 standard which is not already
verified to test against. These sections describe behaviors either during
elaboration or during simulation. Create, then, at most two files and at most
two top-level `sc_module`s, one in each, such that one (which is required) is
named `x.cpp` for some `x`, and the other (which is optional) is named
`x_setup.cpp`. In `x.cpp`, write a module with an `SC_THREAD` named `do_test`
that runs through the test, containing calls to GTest's `EXPECT_` and `ASSERT_`
macros. Additionally in `x.cpp`, there must be at least one test defined that
instantiates the defined module and calls `sc_start`. This may happen many
times, but note that the simulation will be resumed from where it was left off,
which may be after any or none of the other tests. In `x_setup.cpp`, a fixture
should instaniate the object, and various tests may be run against it, without
calling `sc_start`.

All calls to `EXPECT_` and `ASSERT_` must be preceded by a comment that
references by section number and directly quotes the portion of the
specification that prescribes the tested behavior, or "Ibid." if the reasoning
of the immediately preceding `EXPECT_` or `ASSERT_` justifies the one the
comment annotates as well.
