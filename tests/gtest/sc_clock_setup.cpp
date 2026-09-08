#include "tests.h"

#include <cmath>

using namespace testing;

class nan_clock : public sc_core::sc_module {
public:
    sc_core::sc_clock clk;

    SC_CTOR(nan_clock)
        : clk("clk", 1.0, sc_core::SC_NS, NAN) { };
};

class sc_clock_t : public Test {
public:
    sc_clock_t()
        : nc("nc")
    {
    }

    nan_clock nc;
};

TEST_F(sc_clock_t, nan_clock)
{
    // §6.7.4 (b), "The duty cycle shall lie between the limits 0.0 and 1.0,
    // exclusive. The default duty cycle is 0.5."
    EXPECT_EQ(nc.clk.duty_cycle(), 0.5);
}
