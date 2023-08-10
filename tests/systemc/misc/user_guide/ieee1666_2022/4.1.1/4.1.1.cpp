#include <systemc>

struct Mod : sc_core::sc_module {
  SC_CTOR(Mod) {}
};

struct S {
  Mod m;                         // Not recommended coding style - module instance within struct
  S(const char* name_) : m(name_) {}
};

struct Top : sc_core::sc_module { // Five instances of module Mod exist within module Top.
  Mod m1;                        // Recommended coding style
  Mod *m2;                       // Recommended coding style
  S s1;

  SC_CTOR(Top)
  : m1("m1"),                    // m1.name() returns "top.m1"
    s1("s1")                     // s1.m.name() returns "top.s1"
  {
    m2 = new Mod("m2");          // m2->name() returns "top.m2"
    f();
    S *s2 = new S("s2");         // s2->m.name() returns "top.s2"
  }

  void f() {
    Mod *m3 = new Mod("m3");     // Not recommended coding style
  }                              // m3->name() returns "top.m3"
};

int sc_main(int argc, char* argv[]) {
  Top top("top");
  sc_core::sc_start();
  return 0;
}

