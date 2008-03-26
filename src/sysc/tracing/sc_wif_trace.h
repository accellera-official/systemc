/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_wif_trace.h - Implementation of WIF tracing.

  Original Author - Abhijit Ghosh, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL). Though this
   code is somewhat different, and significantly enhanced, the basics
   are identical to what was originally contributed by Infineon.
   The contribution of Infineon in the development of this tracing
   technology is hereby acknowledged.

 *****************************************************************************/

/*****************************************************************************

   Instead of creating the binary WIF format, we create the ASCII
   WIF format which can be converted to the binary format using
   a2wif (utility that comes with VSS from Synopsys). This way, 
   a user who does not have Synopsys VSS can still create WIF 
   files, but the files can only be viewed by users who have VSS.

 *****************************************************************************/

#ifndef SC_WIF_TRACE_H
#define SC_WIF_TRACE_H

#include <cstdio>
#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/tracing/sc_trace.h"
#include "sysc/utils/sc_vector.h"

namespace sc_core {

class wif_trace;  // defined in wif_trace.cc
template<class T> class wif_T_trace;

void wif_put_error_message(const char* msg, bool just_warning);

class wif_trace_file : public sc_trace_file{
public:
    enum wif_enum {WIF_BIT=0, WIF_MVL=1, WIF_REAL=2, WIF_LAST};

    void sc_set_wif_time_unit(int exponent10_seconds); // -7 -> 100ns

    // Create a wif trace file.
    // `Name' forms the base of the name to which `.awif' is added.
    wif_trace_file(const char *name);

    // Flush results and close file.
    ~wif_trace_file();

protected:
    // These are all virtual functions in sc_trace_file and
    // they need to be defined here.

    // Trace a boolean object (single bit)
     void trace(const bool& object, const std::string& name);

    // Trace a sc_bit object (single bit)
     void trace(const sc_dt::sc_bit& object, const std::string& name);

    // Trace a sc_logic object (single bit)
     void trace(const sc_dt::sc_logic& object, const std::string& name);
    
    // Trace an unsigned char with the given width
     void trace(const unsigned char& object, const std::string& name, 
	int width);

    // Trace an unsigned short with the given width
     void trace(const unsigned short& object, const std::string& name, 
         int width);

    // Trace an unsigned int with the given width
     void trace(const unsigned int& object, const std::string& name, 
         int width);

    // Trace an unsigned long with the given width
     void trace(const unsigned long& object, const std::string& name, 
         int width);

    // Trace a signed char with the given width
     void trace(const char& object, const std::string& name, int width);

    // Trace a signed short with the given width
     void trace(const short& object, const std::string& name, int width);

    // Trace a signed int with the given width
     void trace(const int& object, const std::string& name, int width);

    // Trace a signed long with the given width
     void trace(const long& object, const std::string& name, int width);
    
    // Trace a signed long long with the given width
     void trace(const sc_dt::int64& object, const std::string& name, 
         int width);
    
    // Trace an usigned long long with the given width
     void trace(const sc_dt::uint64& object, const std::string& name, 
         int width);
    
    // Trace a float
     void trace(const float& object, const std::string& name);

    // Trace a double
     void trace(const double& object, const std::string& name);

    // Trace sc_unsigned
     void trace (const sc_dt::sc_unsigned& object, 
	 const std::string& name);

    // Trace sc_signed
     void trace (const sc_dt::sc_signed& object, 
	 const std::string& name);

    // Trace sc_uint_base
     void trace (const sc_dt::sc_uint_base& object, 
         const std::string& name);

    // Trace sc_int_base
     void trace (const sc_dt::sc_int_base& object, const std::string& name);

    // Trace sc_fxval
    void trace( const sc_dt::sc_fxval& object, const std::string& name );

    // Trace sc_fxval_fast
    void trace( const sc_dt::sc_fxval_fast& object, 
    	const std::string& name );

    // Trace sc_fxnum
    void trace( const sc_dt::sc_fxnum& object, const std::string& name );

    // Trace sc_fxnum_fast
    void trace( const sc_dt::sc_fxnum_fast& object, 
    	const std::string& name );

    template<class T>
    void traceT(const T& object, const std::string& name, wif_enum type)
    {
      if(initialized)
        wif_put_error_message("No traces can be added once simulation has"
        " started.\nTo add traces, create a new wif trace file.", false);
      else
        traces.push_back(new wif_T_trace<T>(object, name,
          obtain_new_index(),type));
    }

    // Trace sc_bv_base (sc_bv)
    virtual void trace( const sc_dt::sc_bv_base& object, 
    	const std::string& name );

    // Trace sc_lv_base (sc_lv)
    virtual void trace( const sc_dt::sc_lv_base& object, 
    	const std::string& name );

    // Trace an enumerated object - where possible output the enumeration literals
    // in the trace file. Enum literals is a null terminated array of null
    // terminated char* literal strings.
     void trace(const unsigned& object, const std::string& name, 
         const char** enum_literals);

    // Output a comment to the trace file
     void write_comment(const std::string& comment);

    // Also trace transitions between delta cycles if flag is true.
     void delta_cycles(bool flag);

    // Write trace info for cycle.
     void cycle(bool delta_cycle);
    
private:
    // Initialize the tracing mechanism
    void initialize();

    // Create wif names for each variable
    void create_wif_name(std::string* ptr_to_str);

    // Pointer to the file that needs to be written
    FILE* fp;

    double timescale_unit;      // in seconds
    bool timescale_set_by_user; // = 1 means set by user
    bool trace_delta_cycles;    // = 1 means trace the delta cycles

    unsigned wif_name_index;    // Number of variables traced

    unsigned previous_time_units_low, previous_time_units_high; // Previous time as 64 bit integer
    double previous_time;       // Previous time as a double

public:

    bool initialized;           // = 1 means initialized
    // same as create_wif_name (corrected style)
    std::string obtain_new_index();

    // Array to store the variables traced
    sc_pvector<wif_trace*> traces;
};

// Create WIF file
extern sc_trace_file *sc_create_wif_trace_file(const char *name);
extern void sc_close_wif_trace_file( sc_trace_file* tf );

} //  namespace sc_core

#endif
