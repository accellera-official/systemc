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

  test04.cpp -- Test for (un)registering stage callbacks

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

class stage_tracer : sc_module, sc_stage_callback_if
{
public:
  stage_tracer( sc_module_name nm
                  = sc_core::sc_gen_unique_name("stage_tracer") )
    : cb_count(0), timed_count(), delta_count()
  {
    SC_METHOD(timed);
    SC_METHOD(delta);
      sensitive << ev;

    sc_register_stage_callback( *this, SC_POST_BEFORE_END_OF_ELABORATION );
    sc_register_stage_callback( *this, SC_POST_END_OF_ELABORATION );
    sc_register_stage_callback( *this, SC_POST_START_OF_SIMULATION );
    sc_register_stage_callback( *this, SC_POST_UPDATE );
    sc_register_stage_callback( *this, SC_PRE_TIMESTEP );
    sc_register_stage_callback( *this, SC_PRE_PAUSE );
    sc_register_stage_callback( *this, SC_PRE_SUSPEND );  // not tested here
    sc_register_stage_callback( *this, SC_POST_SUSPEND ); // not tested here
    sc_register_stage_callback( *this, SC_PRE_STOP );
    sc_register_stage_callback( *this, SC_POST_END_OF_SIMULATION );
  }

  void timed()
  {
    std::cout
      << sc_get_current_process_handle().name()
      << ": " << sc_time_stamp()
      << ": " << timed_count
      << std::endl;

    if( timed_count++ < 5 ) {
      next_trigger( 100, SC_NS );
    }

    if( delta_count < 5 )
      ev.notify( SC_ZERO_TIME );

    if( timed_count == 5 ) {
      sc_pause();
    }

    if( timed_count >= 6 ) {
      sc_stop();
    }
  }

  void delta()
  {
    std::cout
      << sc_get_current_process_handle().name()
      << ": " << sc_time_stamp()
      << ": " << delta_count
      << std::endl;
    delta_count++;
  }

  virtual void stage_callback(const sc_stage& stage)
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
                << ": stage callback "
                << stage 
                << ": " << sc_time_stamp()
                << " -> pending activity: " << ttp
                << std::endl;
    }
#   endif
    sc_assert( stage );

    switch( stage )
    {
      case SC_POST_UPDATE:   // for SC_POST_UPDATE..
      case SC_PRE_TIMESTEP:  //  ..and SC_PRE_TIMESTEP 
        if( timed_count == 3 )
          ev.cancel();
        if( delta_count == 2 )
          ev.notify(SC_ZERO_TIME);
        if( timed_count == 2 )
          ev.notify( 1, SC_NS );
        break;
      default:
        // do nothing
        break;
    }
  }

  ~stage_tracer()
  { 
    print_static_phase_stats( "[destructor]" ); 

    sc_unregister_stage_callback( *this, SC_POST_BEFORE_END_OF_ELABORATION );
    sc_unregister_stage_callback( *this, SC_POST_END_OF_ELABORATION );
    sc_unregister_stage_callback( *this, SC_POST_START_OF_SIMULATION );
    sc_unregister_stage_callback( *this, SC_POST_UPDATE );
    sc_unregister_stage_callback( *this, SC_PRE_TIMESTEP );
    sc_unregister_stage_callback( *this, SC_PRE_PAUSE );
    sc_unregister_stage_callback( *this, SC_PRE_SUSPEND );
    sc_unregister_stage_callback( *this, SC_POST_SUSPEND );
    sc_unregister_stage_callback( *this, SC_PRE_STOP );
    sc_unregister_stage_callback( *this, SC_POST_END_OF_SIMULATION );
  }

  void print_static_phase_stats( const char* phase )
  {
#if VERBOSE
      std::cout << name()
                << ": " << phase << ": "
                << cb_count << " callbacks called."
                << std::endl;
#endif
  }

private:

  virtual void before_end_of_elaboration()
  {
    sc_assert( sc_get_status() == SC_BEFORE_END_OF_ELABORATION );
    print_static_phase_stats( "before_end_of_elaboration" );
  }

  virtual void end_of_elaboration()
  {
    sc_assert( sc_get_status() == SC_END_OF_ELABORATION );
    print_static_phase_stats( "end_of_elaboration" );
  }

  virtual void start_of_simulation()
  {
    sc_assert( sc_get_status() == SC_START_OF_SIMULATION );
    print_static_phase_stats( "start_of_simulation" );
  }

  virtual void end_of_simulation()
  {
    sc_assert( sc_get_status() == SC_END_OF_SIMULATION );
    print_static_phase_stats( "end_of_simulation" );
  }

private:
  unsigned int cb_mask, old_mask;
  sc_dt::uint64 cb_count, timed_count, delta_count;
  sc_event ev;
};


int sc_main(int, char*[])
{
  stage_tracer tracer("tracer");
  sc_start(); // wait for sc_pause
  sc_start(); // resume, wait for sc_stop
  return 0;
}
