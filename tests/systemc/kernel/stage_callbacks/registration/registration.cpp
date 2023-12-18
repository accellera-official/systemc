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

  register_stage_callbacks.cpp -- Test for (un)registering dynamic callbacks

  Note: requires simulation stage callback support enabled in the kernel
        SC_ENABLE_STAGE_CALLBACKS / --enable-stage-callbacks

  Original Author: Philipp A. Hartmann, OFFIS, 2013-05-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>

#define VERBOSE 1
#define STAGE_ANY (SC_POST_BEFORE_END_OF_ELABORATION | \
                   SC_POST_END_OF_ELABORATION | \
                   SC_POST_START_OF_SIMULATION | \
                   SC_POST_UPDATE | \
                   SC_PRE_TIMESTEP | \
                   SC_PRE_PAUSE | \
                   SC_PRE_SUSPEND | \
                   SC_POST_SUSPEND | \
                   SC_PRE_STOP | \
                   SC_POST_END_OF_SIMULATION)


class stage_tracer : public sc_module, public sc_stage_callback_if
{
public:
  stage_tracer( sc_module_name nm
                  = sc_core::sc_gen_unique_name("stage_tracer") )
    : cb_count(0), timed_count(), delta_count()
  {
    SC_METHOD(timed);
    SC_METHOD(delta);
      sensitive << ev;

    sc_register_stage_callback(*this, STAGE_ANY);

  }

  void timed()
  {
    std::cout
      << sc_get_current_process_handle().name()
      << ": " << sc_time_stamp()
      << ": " << timed_count
      << std::endl;
    if( timed_count++ < 6 ) {
      next_trigger( 100, SC_NS );
    }
    if (timed_count == 4) {
      sc_unregister_stage_callback(*this, SC_PRE_TIMESTEP);
    }
    if( delta_count < 5 )
      ev.notify( SC_ZERO_TIME );

    if( timed_count>=7 )
      sc_stop();
  }
  void delta()
  {
    std::cout
      << sc_get_current_process_handle().name()
      << ": " << sc_time_stamp()
      << ": " << delta_count
      << std::endl;
    delta_count++;
    if (delta_count == 4) {
      sc_unregister_stage_callback(*this, SC_POST_UPDATE);
    }
  }

  void stage_callback(const sc_stage& stage)
  {
    cb_count++;

#   if VERBOSE
    {
      std::string ttp;
      if( !sc_pending_activity() ) {
        ttp = "MAX";
      } else {
        ttp = sc_time_to_pending_activity().to_string();
      }
      std::cout << name()
                << "::stage_callback, sc_status = "
                << sc_get_status()
		<< ", sc_stage = " << stage
                << ", time = " << sc_time_stamp()
                << " -> pending activity: " << ttp
                << std::endl;
    }
#   endif

    switch( stage )
    {
    case SC_POST_START_OF_SIMULATION:
      std::cout << "SC_POST_START_OF_SIMULATION: Notifying event in callback in next delta, expect: Warning: (W553) forbidden action in stage callback" << std::endl;
      ev.notify(SC_ZERO_TIME); // not allowed according to LRM
      std::cout << "After notifying event in callback" << std::endl << std::endl;
      std::cout << "SC_POST_START_OF_SIMULATION: Notifying event in callback with time argument, expect: Warning: (W553) forbidden action in stage callback" << std::endl;
      ev.notify(1, SC_NS); // not allowed according to LRM
      std::cout << "After notifying event in callback" << std::endl << std::endl;
      std::cout << "SC_POST_START_OF_SIMULATION: Notifying event in callback with no argument (direct notification), expect: Warning: (W553) forbidden action in stage callback" << std::endl;
      ev.notify(1, SC_NS); // not allowed according to LRM
      std::cout << "After notifying event in callback" << std::endl << std::endl;
      break;
    case SC_POST_UPDATE:
      if( timed_count == 3 ) {
        std::cout << "SC_POST_UPDATE: Cancelling event in callback, expect no warning message" << std::endl;
        ev.cancel(); // allowed according to LRM
        std::cout << "After cancelling event in callback" << std::endl << std::endl;
      }
      // NOTE: warning is not always produced. 
      // This is due to the early exit in sc_event::notify when m_notify_type == DELTA
      // in that case, the new notification is silently ignored and thus no warning produced
      
      if( delta_count == 2 ) {
        std::cout << "SC_POST_UPDATE: Notifying event in callback in next delta, expect: Warning: (W553) forbidden action in stage callback" << std::endl;
        ev.notify(SC_ZERO_TIME); // not allowed according to LRM
        std::cout << "After notifying event in callback" << std::endl << std::endl;
      }
      if( timed_count == 2 ) {
        std::cout << "SC_POST_UPDATE: Notifying event in callback, expect: Warning: (W553) forbidden action in stage callback" << std::endl;
        ev.notify( 1, SC_NS ); // not allowed according to LRM
        std::cout << "After notifying event in callback" << std::endl << std::endl;
      }
      // NOTE: direct notification in this stage will trigger error: (E521) immediate notification is not allowed during update phase or elaboration
      break;
    case SC_PRE_TIMESTEP:
      if( timed_count == 3 ) {
        std::cout << "SC_PRE_TIMESTEP: Cancelling event in callback, expect no warning message" << std::endl;
        ev.cancel(); // allowed according to LRM
        std::cout << "After cancelling event in callback" << std::endl << std::endl;
      }
      if( delta_count == 2 ) {
        std::cout << "SC_PRE_TIMESTEP: Notifying event in callback in next delta, expect: Warning: (W553) forbidden action in stage callback" << std::endl;
        ev.notify(SC_ZERO_TIME); // not allowed according to LRM
        std::cout << "After notifying event in callback" << std::endl << std::endl;
      }
      if( timed_count == 2) {
        std::cout << "SC_PRE_TIMESTEP: Notifying event in callback, expect: Warning: (W553) forbidden action in stage callback" << std::endl;
        ev.notify( 1, SC_NS ); // not allowed according to LRM
        std::cout << "After notifying event in callback" << std::endl << std::endl;
      }
      // NOTE: direct notification will trigger error: (E521) immediate notification is not allowed during update phase or elaboration
      break;
    default:
      // do nothing
      break;
    }
  }

  ~stage_tracer()
      { print_static_stage_stats( "[destructor]" ); }

  void print_static_stage_stats( const char* stage )
  {
#if VERBOSE
      std::cout << name()
                << ": " << stage << ": "
                << cb_count << " callbacks called."
                << std::endl;
#endif
  }

private:

  virtual void before_end_of_elaboration()
  {
    sc_assert( sc_get_status() == SC_BEFORE_END_OF_ELABORATION );
    print_static_stage_stats( "before_end_of_elaboration" );
  }

  virtual void end_of_elaboration()
  {
    sc_assert( sc_get_status() == SC_END_OF_ELABORATION );
    print_static_stage_stats( "end_of_elaboration" );
  }

  virtual void start_of_simulation()
  {
    sc_assert( sc_get_status() == SC_START_OF_SIMULATION );
    print_static_stage_stats( "start_of_simulation" );

    // This registration is ignored - issues warning (W552) register stage callback: Elaboration done
    std::cout << "start_of_simulation: registering stage_callback for SC_POST_OF_ELABORATION, expect: Warning: (W552) register stage callback: Elaboration done" << std::endl;
    sc_register_stage_callback(*this, SC_POST_END_OF_ELABORATION );
    std::cout << "After ignored registration" << std::endl << std::endl;
  }

  virtual void end_of_simulation()
  {
    sc_assert( sc_get_status() == SC_END_OF_SIMULATION );
    print_static_stage_stats( "end_of_simulation" );
  }



private:
  sc_dt::uint64 cb_count, timed_count, delta_count;
  sc_event ev;
};


int sc_main(int, char*[])
{
  stage_tracer st("my_tracer");
  sc_start();
  return 0;
}
