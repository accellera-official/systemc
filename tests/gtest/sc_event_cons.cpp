#include "tests.h"

using namespace testing;

struct hierarchical_event_container : public sc_core::sc_module {
    SC_CTOR(hierarchical_event_container)
    {
        SC_THREAD(do_test);
    }

    void do_test()
    {
        // "Calling the constructor sc_event(const char*) with an empty string
        // will have the same effect as calling the default constructor,
        // regardless of when it is called by the application." (§5.10.4)
        auto empty_name = sc_core::sc_event("");

        // "When the default constructor is called by the application from the
        // initialization phase onwards, whether or not a hierarchically named
        // event is created shall be implementation-defined on a per-instance
        // basis" (§5.10.4), so we can't check that we're in any particular
        // entry. However, we are guaranteed that "An event that is not
        // hierarchically named shall have a non-empty implementation-defined
        // name." (§5.10.4)
        EXPECT_NE(std::string(empty_name.name()), "");

        // "Member function in_hierarchy shall return the value true if and only
        // if the event is a hierarchically named event" (§5.10.5)
        EXPECT_FALSE(empty_name.in_hierarchy());

        auto e1 = sc_core::sc_event("e1");

        // "Member function in_hierarchy shall return the value true if and only
        // if the event is a hierarchically named event" (§5.10.5)
        EXPECT_TRUE(e1.in_hierarchy());

        auto evs = sc_core::sc_find_object("hec.do_test")->get_child_events();
        // When the constructor sc_event(const char*) is called by the
        // application from the initialization phase onward with a non-empty
        // string argument, the implementation shall create a hierachically
        // named event. (ibid)
        EXPECT_THAT(evs, ElementsAre(&e1));

        // See the procedure described in §5.17.
        EXPECT_EQ(std::string(e1.name()), "hec.do_test.e1");

        auto e2 = sc_core::sc_event("e1");
        evs = sc_core::sc_find_object("hec.do_test")->get_child_events();

        // "When the constructor sc_event(const char*) is called by the
        // application from the initialization phase onward with a non-empty
        // string argument, the implementation shall create a hierachically
        // named event." (ibid)
        EXPECT_THAT(evs, ElementsAre(&e1, &e2));

        // "Each sc_object and each hierarchically named sc_event shall have a
        // unique non-empty hierarchical name" (§5.17)
        EXPECT_NE(std::string(e2.name()), "hec.do_test.e1");
    }
};

class sc_event_cons_t : public Test {
public:
    sc_event_cons_t()
        : hec("hec")
    {
    }

    hierarchical_event_container hec;
};

TEST_F(sc_event_cons_t, do_test)
{
    sc_core::sc_start();
}
