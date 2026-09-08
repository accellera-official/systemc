#ifndef SYSTEMC_GTEST_TESTS_H
#define SYSTEMC_GTEST_TESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <systemc>

int sc_main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif
