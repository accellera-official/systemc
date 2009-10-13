#include <systemc>
using namespace sc_core;
using namespace sc_dt;
using std::cout;
using std::endl;

// 10) sc_find_object, sc_get_top_level_objects

static int object_count = 0;

void recurse_hierarchy(sc_object* obj)
{
  ++ object_count;
  std::vector<sc_object*> children = obj->get_child_objects();
  for (std::vector<sc_object*>::iterator i = children.begin(); i != children.end(); i++)
    recurse_hierarchy(*i);
}

SC_MODULE(M)
{
  SC_CTOR(M)
    : sig("sig")
  {
    SC_THREAD(T);
  }
  void T()
  {
  }
  sc_signal<int> sig;
};

SC_MODULE(Top)
{
  M *m;
  SC_CTOR(Top)
  {
    m = new M("m");

    sc_object* obj;
    obj = sc_find_object("");
    assert(obj == 0);
    obj = sc_find_object("foo");
    assert(obj == 0);
    obj = sc_find_object("top.m.foo");
    assert(obj == 0);
    obj = sc_find_object("top");
    assert(strcmp(obj->kind(), "sc_module") == 0);
    obj = sc_find_object("top.m");
    assert(strcmp(obj->kind(), "sc_module") == 0);
    obj = sc_find_object("top.m.sig");
    assert(strcmp(obj->kind(), "sc_signal") == 0);
    obj = sc_find_object("top.m.T");
    assert(sc_process_handle(obj).valid());
    assert(strcmp(sc_process_handle(obj).name(), "top.m.T") == 0);

    object_count = 0;
    recurse_hierarchy(m);
    assert(object_count == 3);
  }
};

int sc_main(int argc, char* argv[])
{
  cout << "Should be silent..." << endl;

  std::vector<sc_object*> tops = sc_get_top_level_objects();
  assert(tops.size() == 0);

  Top top("top");

  tops = sc_get_top_level_objects();
  assert(tops.size() == 1);
  assert(strcmp(tops[0]->name(), "top") == 0);

  M m2("m2");

  tops = sc_get_top_level_objects();
  assert(tops.size() == 2);
  assert(strcmp(tops[0]->name(), "top") == 0);
  assert(strcmp(tops[1]->name(), "m2") == 0);

  sc_signal<int> sig("sig2");

  tops = sc_get_top_level_objects();
  assert(tops.size() == 3);
  assert(strcmp(tops[0]->name(), "top") == 0);
  assert(strcmp(tops[1]->name(), "m2") == 0);
  assert(strcmp(tops[2]->name(), "sig2") == 0);

  sc_object* obj;
  obj = sc_find_object("");
  assert(obj == 0);
  obj = sc_find_object("foo");
  assert(obj == 0);
  obj = sc_find_object("top.m.foo");
  assert(obj == 0);
  obj = sc_find_object("top");
  assert(strcmp(obj->kind(), "sc_module") == 0);
  obj = sc_find_object("sig2");
  assert(strcmp(obj->kind(), "sc_signal") == 0);
  obj = sc_find_object("m2");
  assert(strcmp(obj->kind(), "sc_module") == 0);
  obj = sc_find_object("top.m");
  assert(strcmp(obj->kind(), "sc_module") == 0);
  obj = sc_find_object("top.m.sig");
  assert(strcmp(obj->kind(), "sc_signal") == 0);
  obj = sc_find_object("top.m.T");
  assert(sc_process_handle(obj).valid());
  assert(strcmp(sc_process_handle(obj).name(), "top.m.T") == 0);

  object_count = 0;
  recurse_hierarchy(tops[0]);
  assert(object_count == 4);

  sc_start();

  cout << endl << "Success" << endl;
  return 0;
}
