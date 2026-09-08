#include "tests.h"

using namespace testing;

struct hierarchical_event_container : public sc_core::sc_module {
    SC_CTOR(hierarchical_event_container)
        : ev("ev")
    {
    }

    sc_core::sc_event ev;
};

class sc_event_cons_setup_t : public Test {
public:
    sc_event_cons_setup_t()
        : hec("hec")
        , ev("ev")
        , ev_empty_name("")
    {
    }

    hierarchical_event_container hec;
    sc_core::sc_event ev;
    sc_core::sc_event ev_empty_name;
};

TEST_F(sc_event_cons_setup_t, hierarchical_naming)
{
    // Because "When called by the application during elaboration or before the
    // initialization phase, each of the above two constructors shall create a
    // hierarchically named event" (§5.10.4) and "Member function name shall
    // return the hierarchical name of the event instance in the case of a
    // hierarchically named event" (§5.10.5), the following must hold:
    EXPECT_EQ(std::string(hec.ev.name()), "hec.ev");
}

TEST_F(sc_event_cons_setup_t, naming)
{
    // Ibid.
    EXPECT_EQ(std::string(ev.name()), "ev");
}

TEST_F(sc_event_cons_setup_t, empty_naming)
{
    // "The name shall always be non-empty" (§5.10.5). One could argue that this
    // is guarded by the previous if-statement, but we further have that "Each
    // sc_object and each hierarchically named sc_event shall have a unique
    // non-empty hierarchical name" (§5.17).
    EXPECT_NE(std::string(ev_empty_name.name()), "");

    // "When a hierarchically named event is constructed, if a non-empty string
    // is passed as a constructor argument, that string shall be used to set the
    // string name of the event. Otherwise, the string name shall be set to
    // 'event'." (§5.10.4) Note that this is the "string name," which is
    // then manipulated by sc_gen_unique_name.
    EXPECT_THAT(std::string(ev_empty_name.basename()), StartsWith("event"));
}
