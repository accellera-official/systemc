/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/
/*****************************************************************************

  hierarchy_scope.cpp -- Test SystemC hierarchy manipulation API
  Original Author: Philipp A. Hartmann, Intel

 *****************************************************************************/
/*****************************************************************************
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc>

struct leaf_object : public sc_core::sc_object
{
  explicit leaf_object(const char* nm)
    : sc_core::sc_object(nm)
  {}

  // allow public access to hierarchy
  using sc_core::sc_object::get_hierarchy_scope;
};

SC_MODULE(hier_module)
{
  SC_CTOR(hier_module) {}

  hier_module* add_module(const char* nm)
    { return add_named<hier_module>(nm); }
  leaf_object* add_object(const char* nm)
    { return add_named<leaf_object>(nm); }
  sc_core::sc_event* add_event(const char* nm)
    { return add_named<sc_core::sc_event>(nm); }

  ~hier_module()
  {
    // delete all objects/events that have been externally added
    const std::vector<sc_core::sc_object*>& objs = get_child_objects();
    for( unsigned int i=0; i < objs.size(); ++i ) {
      // need casting due to sc_object destructor being protected
      hier_module* m = dynamic_cast<hier_module*>(objs[i]);
      if( m != NULL )
        delete m;
      else
        delete dynamic_cast<leaf_object*>(objs[i]);
    }
    const std::vector<sc_core::sc_event*>& evs = get_child_events();
    for( unsigned int i=0;  i<evs.size(); ++i )
      delete evs[i];
  }

private:
  template<typename T> T* add_named(const char* nm);
  void process();
};

template<typename T>
T* hier_module::add_named(const char* nm)
{
  // force current SystemC hierarchy to point to current module
  sc_core::sc_hierarchy_scope scope = get_hierarchy_scope();

  sc_assert( sc_core::sc_get_current_object() == this );
  T* ret = new T(nm);
  sc_assert( ret->get_parent_object() == this );
  return ret;
}

static void
dump_hierarchy(
    std::vector< sc_core::sc_event* >  const & evs  = sc_core::sc_get_top_level_events()
  , std::vector< sc_core::sc_object* > const & objs = sc_core::sc_get_top_level_objects()
  , unsigned level = 0
)
{
  if (!level)
    std::cout << "------ " << sc_core::sc_get_status()
              << " ------" << std::endl;

  std::vector<sc_core::sc_event*>::const_iterator eit = evs.begin();
  for( ; eit != evs.end(); ++eit )
    std::cout << std::string( level + 1, ' ' )
              << (*eit)->name() << " (sc_event)"
              << std::endl;

  std::vector<sc_core::sc_object*>::const_iterator oit = objs.begin();
  for( ; oit != objs.end(); ++oit )
  {
    std::cout << std::string( level + 1, ' ' )
              << (*oit)->name() << " (" << (*oit)->kind() << ")"
              << std::endl;

    dump_hierarchy( (*oit)->get_child_events()
                  , (*oit)->get_child_objects(), level+1 );
  }
}

SC_MODULE(top_module)
{
  SC_CTOR(top_module)
    : SC_NAMED(m_sub)
    , m_root()
    , m_nested()
    , m_root_ev()
    , m_nested_ev()
  {
    SC_THREAD(process);

    sc_assert( sc_core::sc_get_current_object() == this );
    sc_core::sc_object* sub_obj = m_sub.add_object("ctor_obj");
    sc_core::sc_event*  sub_ev  = m_sub.add_event("ctor_ev");
    sc_assert( sub_obj->get_parent_object() == &m_sub );
    sc_assert( sub_ev->get_parent_object() == &m_sub );
  }

  ~top_module()
  {
    delete m_root;
    delete m_root_ev;
  }

private:
  void before_end_of_elaboration();
  void process();

private:
  hier_module  m_sub;
  hier_module* m_root;
  hier_module* m_nested;
  sc_core::sc_event* m_root_ev;
  sc_core::sc_event* m_nested_ev;
};

void top_module::before_end_of_elaboration()
{
  sc_assert( sc_core::sc_get_current_object() == this );
  sc_assert( m_sub.get_parent_object() == this );

  hier_module SC_NAMED(beoe); // deleted by the end of the function
  sc_assert( beoe.get_parent_object() == this );

  // new SystemC root scope
  {
    sc_core::sc_hierarchy_scope scope( sc_core::sc_hierarchy_scope::get_root() );
    sc_assert( sc_core::sc_get_current_object() == NULL );

    m_root = new hier_module("root");
    sc_assert( m_root->get_parent_object() == NULL );
    m_root->add_module("mod")->add_event("ev");
    m_root->add_object("obj");
    sc_assert( sc_core::sc_find_object("root.obj") != NULL );
    sc_assert( sc_core::sc_find_object("root.obj")->get_parent_object() == m_root );

    // nested hierarchy override
    sc_core::sc_object* beoe_nested_o = beoe.add_object("obj");
    sc_core::sc_event*  beoe_nested_e = beoe.add_event("ev");
    sc_assert( beoe_nested_o->get_parent_object() == &beoe );
    sc_assert( beoe_nested_e->get_parent_object() == &beoe );
  }

  // hierarchy has been restored
  sc_assert( sc_core::sc_get_current_object() == this );

  // explicit modification of existing hierarchy
  leaf_object* beoe_nested_o = m_sub.add_object("obj");
  sc_assert( beoe_nested_o->get_parent_object() == &m_sub );
  {
    sc_core::sc_hierarchy_scope scope = beoe_nested_o->get_hierarchy_scope();
    sc_assert( sc_core::sc_get_current_object() == &m_sub );

    hier_module* beoe_nested_m = new hier_module("beoe_nested_m");
    sc_assert( beoe_nested_m->get_parent_object() == &m_sub );
    // will be deleted by m_sub
  }

  dump_hierarchy();
}

void top_module::process()
{
  sc_core::sc_process_handle hnd = sc_core::sc_get_current_process_handle();
  sc_assert( sc_core::sc_get_current_object() == hnd.get_process_object() );

  leaf_object*       proc_o = m_sub.add_object("proc_obj");
  sc_core::sc_event* proc_e = m_sub.add_event("proc_ev");
  sc_assert( proc_o->get_parent_object() == &m_sub);
  sc_assert( proc_e->get_parent_object() == &m_sub);

  leaf_object       SC_NAMED(obj);
  sc_core::sc_event SC_NAMED(ev);

  dump_hierarchy();
}

int sc_main(int argc, char* argv[])
{
  top_module SC_NAMED(top);
  dump_hierarchy();
  sc_core::sc_start();
  sc_core::sc_stop();
  dump_hierarchy();
  std::cout << "Program completed" << std::endl;
  return 0;
}
