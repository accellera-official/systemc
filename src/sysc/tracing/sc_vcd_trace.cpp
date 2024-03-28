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

  sc_vcd_trace.cpp - Implementation of VCD tracing.

  Original Author - Abhijit Ghosh, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Ali Dasdan, Synopsys, Inc.
  Description of Modification: - Replaced 'width' of sc_(u)int with their
                                 'bitwidth()'.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL). Though this
   code is somewhat different, and significantly enhanced, the basics
   are identical to what was originally contributed by Infineon.  The
   contribution of Infineon in the development of this tracing
   technology is hereby acknowledged.

 *****************************************************************************/


#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_ver.h"
#include "sysc/kernel/sc_event.h"
#include "sysc/datatypes/bit/sc_bit.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/datatypes/bit/sc_lv_base.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/datatypes/int/sc_vector_utils.h"
#include "sysc/datatypes/int/sc_signed_inlines.h"
#include "sysc/datatypes/int/sc_signed_ops.h"
#include "sysc/datatypes/int/sc_unsigned_inlines.h"
#include "sysc/datatypes/fx/fx.h"
#include "sysc/tracing/sc_vcd_trace.h"
#include "sysc/utils/sc_report.h" // sc_assert
#include "sysc/utils/sc_string_view.h"

#include <iomanip>
#include <map>
#include <sstream>

#if defined(_MSC_VER)
# pragma warning(disable:4309) // truncation of constant value
#endif

namespace sc_core {

// Forward declarations for functions that come later in the file
// Map sc_dt::sc_logic to printable VCD
static char map_sc_logic_state_to_vcd_state(char in_char);

// Remove name problems associated with [] in vcd names
static void remove_vcd_name_problems(vcd_trace const* vcd, std::string& name);

const char* vcd_types[] = { "wire", "real", "event", "time" };


// ----------------------------------------------------------------------------
//  CLASS : vcd_trace
//
//  Base class for VCD traces.
// ----------------------------------------------------------------------------

class vcd_trace
{
public:
    vcd_trace(const std::string& name_, const std::string& vcd_name_);

    // Needs to be pure virtual as has to be defined by the particular
    // type being traced
    virtual void write(FILE* f) = 0;

    virtual void set_width();

    static const char* strip_leading_bits(const char* originalbuf);

    // Comparison function needs to be pure virtual too
    virtual bool changed() = 0;

    // Make this virtual as some derived classes may overwrite
    virtual void print_variable_declaration_line(FILE* f, const char* scoped_name);

    void compose_data_line(char* rawdata, char* compdata, size_t compdata_n);
    std::string compose_line(const std::string& data);

    virtual ~vcd_trace();

    const std::string name;
    const std::string vcd_name;
    vcd_trace_file::vcd_enum vcd_var_type;
    int bit_width;
};


vcd_trace::vcd_trace(const std::string& name_, const std::string& vcd_name_)
  : name(name_)
  , vcd_name(vcd_name_)
  , vcd_var_type(vcd_trace_file::VCD_WIRE)
  , bit_width(0)
{
    /* Intentionally blank */
}

void
vcd_trace::compose_data_line(char* rawdata, char* compdata, size_t compdata_n)
{
    sc_assert(rawdata != compdata);

    if(bit_width == 0)
    {
        compdata[0] = '\0';
    }
    else
    {
        if(bit_width == 1)
        {
            compdata[0] = rawdata[0];
            strcpy(&(compdata[1]), vcd_name.c_str());
        }
        else
        {
            const char* effective_begin = strip_leading_bits(rawdata);
// FIXME: see
// https://github.com/OSCI-WG/systemc/pull/642 
// https://github.com/OSCI-WG/systemc/issues/600
// https://github.com/OSCI-WG/systemc/pull/645
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
            std::snprintf(compdata, compdata_n, "b%s %s", effective_begin, vcd_name.c_str());
#pragma GCC diagnostic pop

        }
    }
}

// same as above but not that ugly
std::string
vcd_trace::compose_line(const std::string& data)
{
  if(bit_width == 0)
    return "";
  if(bit_width == 1)
    return data + vcd_name;
  return std::string("b")+strip_leading_bits(data.c_str())+" "+vcd_name;
}

void
vcd_trace::print_variable_declaration_line(FILE* f, const char* scoped_name)
{
    char buf[2000];

    if ( bit_width <= 0 )
    {
        std::stringstream ss;
        ss << "'" << name << "' has 0 bits";
        SC_REPORT_ERROR( SC_ID_TRACING_OBJECT_IGNORED_, ss.str().c_str() );
        return;
    }

    if ( bit_width == 1 )
    {
        std::snprintf(buf, sizeof(buf), "$var %s  % 3d  %s  %s       $end\n",
                      vcd_types[vcd_var_type],
                      bit_width,
                      vcd_name.c_str(),
                      scoped_name);
    }
    else
    {
        std::snprintf(buf, sizeof(buf), "$var %s  % 3d  %s  %s [%d:0]  $end\n",
                      vcd_types[vcd_var_type],
                      bit_width,
                      vcd_name.c_str(),
                      scoped_name,
                      bit_width-1);
    }

    std::fputs(buf, f);
}

void
vcd_trace::set_width()
{
  /* Intentionally Blank, should be defined for each type separately */
}

const char*
vcd_trace::strip_leading_bits(const char* originalbuf)
{
    //*********************************************************************
    // - Remove multiple leading 0,z,x, and replace by only one
    // - For example,
    //    b000z100    -> b0z100
    //    b00000xxx   -> b0xxx
    //    b000        -> b0
    //    bzzzzz1     -> bz1
    //    bxxxz10     -> xz10
    // - For leading 0's followed by 1, remove all leading 0's
    //    b0000010101 -> b10101

    const char* position = originalbuf;

    if( strlen(originalbuf) < 2 ||
	(originalbuf[0] != 'z' && originalbuf[0] != 'x' &&
	 originalbuf[0] != '0' ))
      return originalbuf;

    char first_char = *position;
    while(*position == first_char)
    {
        position++;
    }

    if(first_char == '0' && *position == '1')
        return position;
    // else
    return position-1;
}

vcd_trace::~vcd_trace()
{
  /* Intentionally Blank */
}


template <class T>
class vcd_T_trace : public vcd_trace
{
  public:

    vcd_T_trace( const T& object_,
		 const std::string& name_,
		 const std::string& vcd_name_,
		 vcd_trace_file::vcd_enum type_ )
    : vcd_trace( name_, vcd_name_ ),
      object( object_ ),
      old_value( object_ )
    {
        sc_assert( type_ < vcd_trace_file::VCD_LAST );
        vcd_var_type = type_;
    }

    void write( FILE* f )
    {
        std::fprintf( f, "%s", compose_line( object.to_string() ).c_str() );
        old_value = object;
    }

    bool changed()
        { return !(object == old_value); }

    void set_width()
        { bit_width = object.length(); }

protected:

    const T& object;
    T        old_value;
};

typedef vcd_T_trace<sc_dt::sc_bv_base> vcd_sc_bv_trace;
typedef vcd_T_trace<sc_dt::sc_lv_base> vcd_sc_lv_trace;

// Trace sc_dt::sc_bv_base (sc_dt::sc_bv)
void
vcd_trace_file::trace(
    const sc_dt::sc_bv_base& object, const std::string& name)
{
   traceT(object,name);
}

// Trace sc_dt::sc_lv_base (sc_dt::sc_lv)
void
vcd_trace_file::trace(
    const sc_dt::sc_lv_base& object, const std::string& name)
{
   traceT(object,name);
}

/*****************************************************************************/

class vcd_sc_event_trace : public vcd_trace {
public:
    vcd_sc_event_trace(const sc_dt::uint64& trigger_stamp_,
                       const std::string& name_,
                       const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();

protected:
    const sc_dt::uint64& trigger_stamp;
    sc_dt::uint64 old_trigger_stamp;
};

vcd_sc_event_trace::vcd_sc_event_trace(const sc_dt::uint64& trigger_stamp_,
                                       const std::string& name_,
                                       const std::string& vcd_name_)
    : vcd_trace(name_, vcd_name_)
    , trigger_stamp(trigger_stamp_), old_trigger_stamp(trigger_stamp_)
{
    vcd_var_type = vcd_trace_file::VCD_EVENT;
    bit_width = 1;
}

bool
vcd_sc_event_trace::changed()
{
    return trigger_stamp != old_trigger_stamp;
}

void
vcd_sc_event_trace::write(FILE* f)
{
    if(!changed()) return;
    std::fprintf(f,"1%s", vcd_name.c_str());
    old_trigger_stamp = trigger_stamp;
}

/*****************************************************************************/

class vcd_bool_trace : public vcd_trace {
public:
    vcd_bool_trace(const bool& object_,
		   const std::string& name_,
		   const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();

protected:
    const bool& object;
    bool old_value;
};

vcd_bool_trace::vcd_bool_trace(const bool& object_,
			       const std::string& name_,
			       const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_)
{
    bit_width = 1;
}

bool
vcd_bool_trace::changed()
{
    return object != old_value;
}

void
vcd_bool_trace::write(FILE* f)
{
    if (object == true) std::fputc('1', f);
    else std::fputc('0', f);

    std::fprintf(f,"%s", vcd_name.c_str());

    old_value = object;
}

//*****************************************************************************

class vcd_sc_bit_trace : public vcd_trace {
public:
    vcd_sc_bit_trace(const sc_dt::sc_bit& , const std::string& ,
    	const std::string& );
    void write(FILE* f);
    bool changed();

protected:
    const sc_dt::sc_bit& object;
    sc_dt::sc_bit old_value;
};

vcd_sc_bit_trace::vcd_sc_bit_trace( const sc_dt::sc_bit& object_,
				    const std::string& name,
				    const std::string& vcd_name)
: vcd_trace(name, vcd_name), object( object_ ), old_value( object_ )
{
    bit_width = 1;
}

bool
vcd_sc_bit_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_bit_trace::write(FILE* f)
{
    if (object == true) std::fputc('1', f);
    else std::fputc('0', f);

    std::fprintf(f,"%s", vcd_name.c_str());

    old_value = object;
}

/*****************************************************************************/

class vcd_sc_logic_trace : public vcd_trace {
public:
    vcd_sc_logic_trace(const sc_dt::sc_logic& object_,
		       const std::string& name_,
		       const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();

protected:
    const sc_dt::sc_logic& object;
    sc_dt::sc_logic old_value;
};


vcd_sc_logic_trace::vcd_sc_logic_trace(const sc_dt::sc_logic& object_,
				       const std::string& name_,
				       const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_)
{
    bit_width = 1;
}


bool
vcd_sc_logic_trace::changed()
{
    return object != old_value;
}


void
vcd_sc_logic_trace::write(FILE* f)
{
    char out_char;
    out_char = map_sc_logic_state_to_vcd_state(object.to_char());
    std::fputc(out_char, f);

    std::fprintf(f,"%s", vcd_name.c_str());

    old_value = object;
}


/*****************************************************************************/

class vcd_sc_unsigned_trace : public vcd_trace {
public:
    vcd_sc_unsigned_trace(const sc_dt::sc_unsigned& object_,
			  const std::string& name_,
			  const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_unsigned& object;
    sc_dt::sc_unsigned old_value;
};


vcd_sc_unsigned_trace::vcd_sc_unsigned_trace(const sc_dt::sc_unsigned& object_,
					     const std::string& name_,
					     const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_.length())
// The last may look strange, but is correct
{
    old_value = object;
}

bool
vcd_sc_unsigned_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_unsigned_trace::write(FILE* f)
{
    static std::vector<char> compdata(1024), rawdata(1024);
    typedef std::vector<char>::size_type size_t;

    if ( compdata.size() <= static_cast<size_t>(object.length()) ) { // include trailing \0
        size_t sz = ( static_cast<size_t>(object.length()) + 4096 ) & (~static_cast<size_t>(4096-1));
        std::vector<char>( sz ).swap( compdata ); // resize without copying values
        std::vector<char>( sz ).swap( rawdata );
    }
    char *rawdata_ptr  = &rawdata[0];

    for (int bitindex = object.length() - 1; bitindex >= 0; --bitindex) {
        *rawdata_ptr++ = "01"[object[bitindex].to_bool()];
    }
    *rawdata_ptr = '\0';
    compose_data_line(&rawdata[0], &compdata[0], compdata.size());

    std::fputs(&compdata[0], f);
    old_value = object;
}

void
vcd_sc_unsigned_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class vcd_sc_signed_trace : public vcd_trace {
public:
    vcd_sc_signed_trace(const sc_dt::sc_signed& object_,
			const std::string& name_,
			const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_signed& object;
    sc_dt::sc_signed old_value;
};


vcd_sc_signed_trace::vcd_sc_signed_trace(const sc_dt::sc_signed& object_,
					 const std::string& name_,
					 const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_.length())
{
    old_value = object;
}

bool
vcd_sc_signed_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_signed_trace::write(FILE* f)
{
    static std::vector<char> compdata(1024), rawdata(1024);
    typedef std::vector<char>::size_type size_t;

    if ( compdata.size() <= static_cast<size_t>(object.length()) ) { // include trailing \0
        size_t sz = ( static_cast<size_t>(object.length()) + 4096 ) & (~static_cast<size_t>(4096-1));
        std::vector<char>( sz ).swap( compdata ); // resize without copying values
        std::vector<char>( sz ).swap( rawdata );
    }
    char *rawdata_ptr  = &rawdata[0];

    for (int bitindex = object.length() - 1; bitindex >= 0; --bitindex) {
        *rawdata_ptr++ = "01"[object[bitindex].to_bool()];
    }
    *rawdata_ptr = '\0';
    compose_data_line(&rawdata[0], &compdata[0], compdata.size());

    std::fputs(&compdata[0], f);
    old_value = object;
}

void
vcd_sc_signed_trace::set_width()
{
    bit_width = object.length();
}

/*****************************************************************************/

class vcd_sc_uint_base_trace : public vcd_trace {
public:
    vcd_sc_uint_base_trace(const sc_dt::sc_uint_base& object_,
			   const std::string& name_,
			   const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_uint_base& object;
    sc_dt::sc_uint_base old_value;
};


vcd_sc_uint_base_trace::vcd_sc_uint_base_trace(
                                          const sc_dt::sc_uint_base& object_,
					  const std::string& name_,
					  const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_.length())
// The last may look strange, but is correct
{
    old_value = object;
}

bool
vcd_sc_uint_base_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_uint_base_trace::write(FILE* f)
{
    char rawdata[1000], *rawdata_ptr = rawdata;
    char compdata[1000];

    int bitindex;
    for (bitindex = object.length()-1; bitindex >= 0; --bitindex) {
        *rawdata_ptr++ = "01"[object[bitindex].to_bool()];
    }
    *rawdata_ptr = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));

    std::fputs(compdata, f);
    old_value = object;
}

void
vcd_sc_uint_base_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class vcd_sc_int_base_trace : public vcd_trace {
public:
    vcd_sc_int_base_trace(const sc_dt::sc_int_base& object_,
			  const std::string& name_,
			  const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_int_base& object;
    sc_dt::sc_int_base old_value;
};


vcd_sc_int_base_trace::vcd_sc_int_base_trace(const sc_dt::sc_int_base& object_,
					     const std::string& name_,
					     const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_.length())
{
    old_value = object;
}

bool
vcd_sc_int_base_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_int_base_trace::write(FILE* f)
{
    char rawdata[1000], *rawdata_ptr = rawdata;
    char compdata[1000];

    int bitindex;
    for (bitindex = object.length()-1; bitindex >= 0; --bitindex) {
        *rawdata_ptr++ = "01"[object[bitindex].to_bool()];
    }
    *rawdata_ptr = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));

    std::fputs(compdata, f);
    old_value = object;
}

void
vcd_sc_int_base_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class vcd_sc_fxval_trace : public vcd_trace
{
public:

    vcd_sc_fxval_trace( const sc_dt::sc_fxval& object_,
			const std::string& name_,
			const std::string& vcd_name_ );
    void write( FILE* f );
    bool changed();

protected:

    const sc_dt::sc_fxval& object;
    sc_dt::sc_fxval old_value;

};

vcd_sc_fxval_trace::vcd_sc_fxval_trace( const sc_dt::sc_fxval& object_,
				        const std::string& name_,
					const std::string& vcd_name_ )
: vcd_trace( name_, vcd_name_ ),
  object( object_ ), old_value( object_ )
{
    vcd_var_type = vcd_trace_file::VCD_REAL;
    bit_width = 1;
}

bool
vcd_sc_fxval_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_fxval_trace::write( FILE* f )
{
    std::fprintf( f, "r%.16g %s", object.to_double(), vcd_name.c_str() );
    old_value = object;
}

/*****************************************************************************/

class vcd_sc_fxval_fast_trace : public vcd_trace
{
public:

    vcd_sc_fxval_fast_trace( const sc_dt::sc_fxval_fast& object_,
			     const std::string& name_,
			     const std::string& vcd_name_ );
    void write( FILE* f );
    bool changed();

protected:

    const sc_dt::sc_fxval_fast& object;
    sc_dt::sc_fxval_fast old_value;

};

vcd_sc_fxval_fast_trace::vcd_sc_fxval_fast_trace(
                                        const sc_dt::sc_fxval_fast& object_,
					const std::string& name_,
					const std::string& vcd_name_ )
: vcd_trace( name_, vcd_name_ ),
  object( object_ ), old_value( object_ )
{
    vcd_var_type = vcd_trace_file::VCD_REAL;
    bit_width = 1;
}

bool
vcd_sc_fxval_fast_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_fxval_fast_trace::write( FILE* f )
{
    std::fprintf( f, "r%.16g %s", object.to_double(), vcd_name.c_str() );
    old_value = object;
}

/*****************************************************************************/

class vcd_sc_fxnum_trace : public vcd_trace
{
public:

    vcd_sc_fxnum_trace( const sc_dt::sc_fxnum& object_,
			const std::string& name_,
			const std::string& vcd_name_ );
    void write( FILE* f );
    bool changed();
    void set_width();

protected:

    const sc_dt::sc_fxnum& object;
    sc_dt::sc_fxnum old_value;

};

vcd_sc_fxnum_trace::vcd_sc_fxnum_trace( const sc_dt::sc_fxnum& object_,
				        const std::string& name_,
					const std::string& vcd_name_ )
: vcd_trace( name_, vcd_name_ ),
  object( object_ ),
  old_value( object_.m_params.type_params(),
	     object_.m_params.enc(),
	     object_.m_params.cast_switch(),
	     0 )
{
    old_value = object;
}

bool
vcd_sc_fxnum_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_fxnum_trace::write( FILE* f )
{
    static std::vector<char> compdata(1024), rawdata(1024);
    typedef std::vector<char>::size_type size_t;

    if ( compdata.size() <= static_cast<size_t>(object.wl()) ) { // include trailing \0
        size_t sz = ( static_cast<size_t>(object.wl()) + 4096 ) & (~static_cast<size_t>(4096-1));
        std::vector<char>( sz ).swap( compdata ); // resize without copying values
        std::vector<char>( sz ).swap( rawdata );
    }
    char *rawdata_ptr  = &rawdata[0];

    for(int bitindex = object.wl() - 1; bitindex >= 0; -- bitindex )
    {
        *rawdata_ptr ++ = "01"[object[bitindex]];
    }
    *rawdata_ptr = '\0';
    compose_data_line(&rawdata[0], &compdata[0], compdata.size());

    std::fputs( &compdata[0], f );
    old_value = object;
}

void
vcd_sc_fxnum_trace::set_width()
{
    bit_width = object.wl();
}

/*****************************************************************************/

class vcd_sc_fxnum_fast_trace : public vcd_trace
{
public:

    vcd_sc_fxnum_fast_trace( const sc_dt::sc_fxnum_fast& object_,
			     const std::string& name_,
			     const std::string& vcd_name_ );
    void write( FILE* f );
    bool changed();
    void set_width();

protected:

    const sc_dt::sc_fxnum_fast& object;
    sc_dt::sc_fxnum_fast old_value;

};

vcd_sc_fxnum_fast_trace::vcd_sc_fxnum_fast_trace(
                                        const sc_dt::sc_fxnum_fast& object_,
					const std::string& name_,
					const std::string& vcd_name_ )
: vcd_trace( name_, vcd_name_ ),
  object( object_ ),
  old_value( object_.m_params.type_params(),
	     object_.m_params.enc(),
	     object_.m_params.cast_switch(),
	     0 )
{
    old_value = object;
}

bool
vcd_sc_fxnum_fast_trace::changed()
{
    return object != old_value;
}

void
vcd_sc_fxnum_fast_trace::write( FILE* f )
{
    static std::vector<char> compdata(1024), rawdata(1024);
    typedef std::vector<char>::size_type size_t;

    if ( compdata.size() <= static_cast<size_t>(object.wl()) ) { // include trailing \0
        size_t sz = ( static_cast<size_t>(object.wl()) + 4096 ) & (~static_cast<size_t>(4096-1));
        std::vector<char>( sz ).swap( compdata ); // resize without copying values
        std::vector<char>( sz ).swap( rawdata );
    }
    char *rawdata_ptr  = &rawdata[0];

    for(int bitindex = object.wl() - 1; bitindex >= 0; -- bitindex )
    {
        *rawdata_ptr ++ = "01"[object[bitindex]];
    }
    *rawdata_ptr = '\0';
    compose_data_line(&rawdata[0], &compdata[0], compdata.size());

    std::fputs( &compdata[0], f );
    old_value = object;
}

void
vcd_sc_fxnum_fast_trace::set_width()
{
    bit_width = object.wl();
}


/*****************************************************************************/

class vcd_unsigned_int_trace : public vcd_trace {
public:
    vcd_unsigned_int_trace(const unsigned& object_,
			   const std::string& name_,
			   const std::string& vcd_name_,
			   int width_);
    void write(FILE* f);
    bool changed();

protected:
    const unsigned& object;
    unsigned old_value;
    unsigned mask;
};


vcd_unsigned_int_trace::vcd_unsigned_int_trace(
                                            const unsigned& object_,
					    const std::string& name_,
					    const std::string& vcd_name_,
					    int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value( object_ ),
  mask(~0U)
{
    bit_width = width_;
    if (bit_width < 32) mask = ~(~0U << bit_width);
}


bool
vcd_unsigned_int_trace::changed()
{
    return object != old_value;
}


void
vcd_unsigned_int_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if ((object & mask) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++){
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned bit_mask = 1 << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_unsigned_short_trace : public vcd_trace {
public:
    vcd_unsigned_short_trace(const unsigned short& object_,
			     const std::string& name_,
			     const std::string& vcd_name_,
			     int width_);
    void write(FILE* f);
    bool changed();

protected:
    const unsigned short& object;
    unsigned short old_value;
    unsigned short mask;
};


vcd_unsigned_short_trace::vcd_unsigned_short_trace(
                                        const unsigned short& object_,
					const std::string& name_,
					const std::string& vcd_name_,
					int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_), mask(static_cast<unsigned short>(~0U))
{
    bit_width = width_;
    if (bit_width < 16) mask = static_cast<unsigned short>(~(~0U << bit_width));
}


bool
vcd_unsigned_short_trace::changed()
{
    return object != old_value;
}


void
vcd_unsigned_short_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if ((object & mask) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++){
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned bit_mask = 1 << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_unsigned_char_trace : public vcd_trace {
public:
    vcd_unsigned_char_trace(const unsigned char& object_,
			    const std::string& name_,
			    const std::string& vcd_name_,
			    int width_);
    void write(FILE* f);
    bool changed();

protected:
    const unsigned char& object;
    unsigned char old_value;
    unsigned char mask;
};


vcd_unsigned_char_trace::vcd_unsigned_char_trace(
                                const unsigned char& object_,
				const std::string& name_,
				const std::string& vcd_name_,
				int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_), mask(static_cast<unsigned char>(~0U))
{
    bit_width = width_;
    if (bit_width < 8) mask = static_cast<unsigned char>(~(~0U << bit_width));
}


bool vcd_unsigned_char_trace::changed()
{
    return object != old_value;
}


void vcd_unsigned_char_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if ((object & mask) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++){
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned bit_mask = 1 << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_unsigned_long_trace : public vcd_trace {
public:
    vcd_unsigned_long_trace(const unsigned long& object_,
			    const std::string& name_,
			    const std::string& vcd_name_,
			    int width_);
    void write(FILE* f);
    bool changed();

protected:
    const unsigned long& object;
    unsigned long old_value;
    unsigned long mask;
};


vcd_unsigned_long_trace::vcd_unsigned_long_trace(
                                const unsigned long& object_,
				const std::string& name_,
				const std::string& vcd_name_,
				int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_),
  mask(~0UL)
{
    bit_width = width_;
    if ( bit_width < static_cast<int>(sizeof(unsigned long)*8) )
        mask = ~(~0UL << bit_width);
}


bool vcd_unsigned_long_trace::changed()
{
    return object != old_value;
}


void vcd_unsigned_long_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if ((object & mask) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++){
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned long bit_mask = 1ul << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_signed_int_trace : public vcd_trace {
public:
    vcd_signed_int_trace(const int& object_,
			 const std::string& name_,
			 const std::string& vcd_name_,
			 int width_);
    void write(FILE* f);
    bool changed();

protected:
    const int& object;
    int old_value;
    const unsigned rem_bits;
};


vcd_signed_int_trace::vcd_signed_int_trace(const signed& object_,
					   const std::string& name_,
					   const std::string& vcd_name_,
					   int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_),
  rem_bits(32 - width_)
{
    bit_width = width_;
}


bool vcd_signed_int_trace::changed()
{
    return object != old_value;
}


void vcd_signed_int_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if (((object << rem_bits) >> rem_bits) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned bit_mask = 1 << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_signed_short_trace : public vcd_trace {
public:
    vcd_signed_short_trace(const short& object_,
			   const std::string& name_,
			   const std::string& vcd_name_,
			   int width_);
    void write(FILE* f);
    bool changed();

protected:
    const short& object;
    short old_value;
    const unsigned rem_bits;
};


vcd_signed_short_trace::vcd_signed_short_trace(
					const short& object_,
					const std::string& name_,
					const std::string& vcd_name_,
					int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_), rem_bits(32 - width_)
{
    bit_width = width_;
}


bool vcd_signed_short_trace::changed()
{
    return object != old_value;
}


void vcd_signed_short_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if (((object << rem_bits) >> rem_bits) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned bit_mask = 1 << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_signed_char_trace : public vcd_trace {
public:
    vcd_signed_char_trace(const char& object_,
			  const std::string& name_,
			  const std::string& vcd_name_,
			  int width_);
    void write(FILE* f);
    bool changed();

protected:
    const char& object;
    char old_value;
    const unsigned rem_bits;
};


vcd_signed_char_trace::vcd_signed_char_trace(const char& object_,
					     const std::string& name_,
					     const std::string& vcd_name_,
					     int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_), rem_bits(32 - width_)
{
    bit_width = width_;
}


bool vcd_signed_char_trace::changed()
{
    return object != old_value;
}


void vcd_signed_char_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if (((object << rem_bits) >> rem_bits) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned bit_mask = 1 << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_int64_trace : public vcd_trace {
public:
    vcd_int64_trace(const sc_dt::int64& object_,
			  const std::string& name_,
			  const std::string& vcd_name_,
			  int width_);
    void write(FILE* f);
    bool changed();

protected:
    const sc_dt::int64& object;
    sc_dt::int64 old_value;
    const unsigned rem_bits;
};


vcd_int64_trace::vcd_int64_trace(const sc_dt::int64& object_,
					     const std::string& name_,
					     const std::string& vcd_name_,
					     int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_),
  rem_bits(64 - width_)
{
    bit_width = width_;
}


bool vcd_int64_trace::changed()
{
    return object != old_value;
}


void vcd_int64_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if (((object << rem_bits) >> rem_bits) != object)
    {
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = 'x';
        }
    }
    else
    {
        sc_dt::uint64 bit_mask = 1;
        bit_mask = bit_mask << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}


/*****************************************************************************/

class vcd_uint64_trace : public vcd_trace {
public:
    vcd_uint64_trace(const sc_dt::uint64& object_,
		     const std::string& name_,
		     const std::string& vcd_name_,
		     int width_);
    void write(FILE* f);
    bool changed();

protected:
    const sc_dt::uint64& object;
    sc_dt::uint64 old_value;
    sc_dt::uint64 mask;
};


vcd_uint64_trace::vcd_uint64_trace(  const sc_dt::uint64& object_,
				     const std::string& name_,
				     const std::string& vcd_name_,
				     int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_),
  mask((sc_dt::uint64)-1)
{
    bit_width = width_;
    if ( bit_width < 64 ) mask = ~(mask << bit_width);
}


bool vcd_uint64_trace::changed()
{
    return object != old_value;
}


void vcd_uint64_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if ((object & mask) != object)
    {
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = 'x';
        }
    }
    else
    {
        sc_dt::uint64 bit_mask = 1;
        bit_mask = bit_mask << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************/

class vcd_sc_time_trace : public vcd_uint64_trace
{
public:
    vcd_sc_time_trace(const sc_time& object_,
                      const std::string& name_,
                      const std::string& vcd_name_);

    bool changed();

    const sc_time& object;
    sc_dt::uint64 shadow_object; // trace raw value internally
};

vcd_sc_time_trace::vcd_sc_time_trace( const sc_time& object_,
                                      const std::string& name_,
                                      const std::string& vcd_name_ )
  // initialize shadow_object before passing its reference to base class
  : vcd_uint64_trace( (shadow_object = object_.value()), name_, vcd_name_, 64 )
  , object(object_)
{
    vcd_var_type = vcd_trace_file::VCD_TIME;
}

bool vcd_sc_time_trace::changed()
{
    shadow_object = object.value();
    return vcd_uint64_trace::changed();
}

/*****************************************************************************/

class vcd_signed_long_trace : public vcd_trace {
public:
    vcd_signed_long_trace(const long& object_,
			  const std::string& name_,
			  const std::string& vcd_name_,
			  int width_);
    void write(FILE* f);
    bool changed();

protected:
    const long& object;
    long old_value;
    const unsigned rem_bits;
};


vcd_signed_long_trace::vcd_signed_long_trace(const long& object_,
					     const std::string& name_,
					     const std::string& vcd_name_,
					     int width_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_),
  rem_bits(std::numeric_limits<unsigned long>::digits - width_)
{
    bit_width = width_;
}


bool vcd_signed_long_trace::changed()
{
    return object != old_value;
}


void vcd_signed_long_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if (((object << rem_bits) >> rem_bits) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = 'x';
        }
    }
    else {
        unsigned long bit_mask = 1ul << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}


/*****************************************************************************/

class vcd_float_trace : public vcd_trace {
public:
    vcd_float_trace(const float& object_,
		    const std::string& name_,
		    const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();

protected:
    const float& object;
    float old_value;
};

vcd_float_trace::vcd_float_trace(const float& object_,
				 const std::string& name_,
				 const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_)
{
    vcd_var_type = vcd_trace_file::VCD_REAL;
    bit_width = 1;
    old_value = object;
}

bool vcd_float_trace::changed()
{
    return object != old_value;
}

void vcd_float_trace::write(FILE* f)
{
    std::fprintf(f, "r%.16g %s", object, vcd_name.c_str());
    old_value = object;
}

/*****************************************************************************/

class vcd_double_trace : public vcd_trace {
public:
    vcd_double_trace(const double& object_,
		     const std::string& name_,
		     const std::string& vcd_name_);
    void write(FILE* f);
    bool changed();

protected:
    const double& object;
    double old_value;
};

vcd_double_trace::vcd_double_trace(const double& object_,
				   const std::string& name_,
				   const std::string& vcd_name_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_)
{
    vcd_var_type = vcd_trace_file::VCD_REAL;
    bit_width = 1;
}

bool vcd_double_trace::changed()
{
    return object != old_value;
}

void vcd_double_trace::write(FILE* f)
{
    std::fprintf(f, "r%.16g %s", object, vcd_name.c_str());
    old_value = object;
}


/*****************************************************************************/

class vcd_enum_trace : public vcd_trace {
public:
    vcd_enum_trace(const unsigned& object_,
		   const std::string& name_,
		   const std::string& vcd_name_,
		   const char** enum_literals);
    void write(FILE* f);
    bool changed();

protected:
    const unsigned& object;
    unsigned old_value;
    unsigned mask;
    const char** literals;
    unsigned nliterals;
};


vcd_enum_trace::vcd_enum_trace(const unsigned& object_,
			       const std::string& name_,
			       const std::string& vcd_name_,
			       const char** enum_literals_)
: vcd_trace(name_, vcd_name_), object(object_), old_value(object_),
  mask(~0U), literals(enum_literals_), nliterals(0)
{
    // find number of bits required to represent enumeration literal - counting loop
    for (nliterals = 0; enum_literals_[nliterals]; nliterals++) continue;

    // Figure out number of bits required to represent the number of literals
    bit_width = 0;
    unsigned shifted_maxindex = nliterals-1;
    while(shifted_maxindex != 0){
        shifted_maxindex >>= 1;
        bit_width++;
    }

    // Set the mask
    if (bit_width < 32) {
      mask = ~(~0U << bit_width);
    } else {
      mask = ~0U;
    }
}

bool vcd_enum_trace::changed()
{
    return object != old_value;
}

void vcd_enum_trace::write(FILE* f)
{
    char rawdata[1000];
    char compdata[1000];
    int bitindex;

    // Check for overflow
    if ((object & mask) != object) {
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = 'x';
        }
    } else {
        unsigned long bit_mask = 1ul << (bit_width-1);
        for (bitindex = 0; bitindex < bit_width; bitindex++) {
            rawdata[bitindex] = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    rawdata[bitindex] = '\0';
    compose_data_line(rawdata, compdata, sizeof(compdata));
    std::fputs(compdata, f);
    old_value = object;
}

/*****************************************************************************
 VCD Scopes support
 *****************************************************************************/

struct vcd_scope {

    void add_trace(vcd_trace *trace, bool with_scopes);
    void print(FILE *fp, const char *scope_name = "SystemC");

    ~vcd_scope();
private:
    void add_trace_rec(std::stringstream &ss, const std::string &cur_name, vcd_trace *trace);

    std::vector<std::pair<std::string,vcd_trace*> > m_traces;
    std::map<std::string, vcd_scope*> m_scopes;
};


vcd_scope::~vcd_scope() {
    for (std::map<std::string, vcd_scope*>::iterator it = m_scopes.begin(); it != m_scopes.end(); ++it)
        delete (*it).second;
}

void vcd_scope::add_trace(vcd_trace *trace, bool with_scopes)
{
    std::string name_copy = trace->name;
    remove_vcd_name_problems(trace, name_copy);

    if (with_scopes) {
        std::stringstream ss(name_copy);
        std::string first_token;
        std::getline(ss, first_token, '.');
        add_trace_rec(ss, first_token, trace);
    } else {
        m_traces.push_back(std::make_pair(name_copy, trace));
    }

}

void vcd_scope::add_trace_rec(std::stringstream &ss, const std::string &cur_name, vcd_trace *trace) {
    std::string next_token;
    if (std::getline(ss, next_token, '.')) {
        vcd_scope*& cur_scope = m_scopes[cur_name];
        if (!cur_scope)
            cur_scope = new vcd_scope;
        cur_scope->add_trace_rec(ss, next_token, trace);
    } else {
        m_traces.push_back(std::make_pair(cur_name, trace));
    }
}

void vcd_scope::print(FILE *fp, const char *scope_name) {
    fprintf(fp,"$scope module %s $end\n", scope_name);

    for (std::vector<std::pair<std::string,vcd_trace*> >::iterator it = m_traces.begin(); it != m_traces.end(); ++it) {
        it->second->set_width();
        it->second->print_variable_declaration_line(fp, it->first.c_str());
    }

    for (std::map<std::string, vcd_scope*>::iterator it = m_scopes.begin(); it != m_scopes.end(); ++it)
        it->second->print(fp,it->first.c_str());

    fprintf(fp,"$upscope $end\n");
}

#ifdef SC_DISABLE_VCD_SCOPES
#  define VCD_SCOPES_DEFAULT_ false
#else
#  define VCD_SCOPES_DEFAULT_ true
#endif

void vcd_print_scopes(FILE *fp, std::vector<vcd_trace*>& traces) {

    vcd_scope top_scope;

    const char*    with_scopes_p = std::getenv("SC_VCD_SCOPES");
    sc_string_view with_scopes_s = (with_scopes_p) ? with_scopes_p : "";

    bool with_scopes = VCD_SCOPES_DEFAULT_;
    if (with_scopes_s == "DISABLE") with_scopes = false;
    if (with_scopes_s == "ENABLE")  with_scopes = true;

    for (std::vector<vcd_trace*>::iterator it = traces.begin(); it != traces.end(); ++it)
        top_scope.add_trace(*it, with_scopes);

    top_scope.print(fp);
}


/*****************************************************************************
           vcd_trace_file functions
 *****************************************************************************/

vcd_trace_file::vcd_trace_file(const char *name)
  : sc_trace_file_base( name, "vcd" )
  , vcd_name_index(0)
  , previous_time_units_low(0)
  , previous_time_units_high(0)
  , traces()
{}


void
vcd_trace_file::do_initialize()
{
    //date:
    std::fprintf(fp, "$date\n     %s\n$end\n\n", localtime_string().c_str() );

    //version:
    std::fprintf(fp, "$version\n %s\n$end\n\n", sc_version());

    //timescale:
    std::fprintf(fp,"$timescale\n     %s\n$end\n\n", fs_unit_to_str(trace_unit_fs).c_str());

    vcd_print_scopes(fp, traces);

    std::fputs("$enddefinitions  $end\n\n", fp);

    timestamp_in_trace_units(previous_time_units_high, previous_time_units_low);

    std::stringstream ss;

    ss << "All initial values are dumped below at time "
       << sc_time_stamp().to_seconds() <<" sec = ";
    if(has_low_units())
        ss << previous_time_units_high << std::setfill('0') << std::setw(low_units_len()) << previous_time_units_low;
    else
        ss << previous_time_units_high;
    ss << " timescale units.";

    write_comment(ss.str());

    std::fputs("$dumpvars\n",fp);
    for (int i = 0; i < (int)traces.size(); i++) {
        traces[i]->write(fp);
        std::fputc('\n', fp);
    }
    std::fputs("$end\n\n", fp);
}

void vcd_trace_file::trace( sc_trace_file* ) const {
    SC_REPORT_ERROR( sc_core::SC_ID_INTERNAL_ERROR_
                   , "invalid call to vcd_trace_file::trace(sc_trace_file*)" );
}

// ----------------------------------------------------------------------------

#define DEFN_TRACE_METHOD(tp)                                                 \
void                                                                          \
vcd_trace_file::trace(const tp& object_, const std::string& name_)            \
{                                                                             \
    if( add_trace_check(name_) )                                              \
        traces.push_back( new vcd_ ## tp ## _trace( extract_ref(object_),     \
                                                    name_,                    \
                                                    obtain_name() ) );        \
}

DEFN_TRACE_METHOD(sc_event)
DEFN_TRACE_METHOD(sc_time)

DEFN_TRACE_METHOD(bool)
DEFN_TRACE_METHOD(float)
DEFN_TRACE_METHOD(double)

#undef DEFN_TRACE_METHOD
#define DEFN_TRACE_METHOD(tp)                                                 \
void                                                                          \
vcd_trace_file::trace(const sc_dt::tp& object_, const std::string& name_)     \
{                                                                             \
    if( add_trace_check(name_) )                                              \
        traces.push_back( new vcd_ ## tp ## _trace( object_,                  \
                                                    name_,                    \
                                                    obtain_name() ) );        \
}

DEFN_TRACE_METHOD(sc_bit)
DEFN_TRACE_METHOD(sc_logic)

DEFN_TRACE_METHOD(sc_signed)
DEFN_TRACE_METHOD(sc_unsigned)
DEFN_TRACE_METHOD(sc_int_base)
DEFN_TRACE_METHOD(sc_uint_base)

DEFN_TRACE_METHOD(sc_fxval)
DEFN_TRACE_METHOD(sc_fxval_fast)
DEFN_TRACE_METHOD(sc_fxnum)
DEFN_TRACE_METHOD(sc_fxnum_fast)

#undef DEFN_TRACE_METHOD


#define DEFN_TRACE_METHOD_SIGNED(tp)                                          \
void                                                                          \
vcd_trace_file::trace( const tp&          object_,                            \
                       const std::string& name_,                              \
                       int                width_ )                            \
{                                                                             \
    if( add_trace_check(name_) )                                              \
        traces.push_back( new vcd_signed_ ## tp ## _trace( object_,           \
                                                           name_,             \
                                                           obtain_name(),     \
                                                           width_ ) );        \
}

#define DEFN_TRACE_METHOD_UNSIGNED(tp)                                        \
void                                                                          \
vcd_trace_file::trace( const unsigned tp& object_,                            \
                       const std::string& name_,                              \
                       int                width_ )                            \
{                                                                             \
    if( add_trace_check(name_) )                                              \
        traces.push_back( new vcd_unsigned_ ## tp ## _trace( object_,         \
                                                             name_,           \
                                                             obtain_name(),   \
                                                             width_ ) );      \
}

DEFN_TRACE_METHOD_SIGNED(char)
DEFN_TRACE_METHOD_SIGNED(short)
DEFN_TRACE_METHOD_SIGNED(int)
DEFN_TRACE_METHOD_SIGNED(long)

DEFN_TRACE_METHOD_UNSIGNED(char)
DEFN_TRACE_METHOD_UNSIGNED(short)
DEFN_TRACE_METHOD_UNSIGNED(int)
DEFN_TRACE_METHOD_UNSIGNED(long)

#undef DEFN_TRACE_METHOD_SIGNED
#undef DEFN_TRACE_METHOD_UNSIGNED

#define DEFN_TRACE_METHOD_LONG_LONG(tp)                                       \
void                                                                          \
vcd_trace_file::trace( const sc_dt::tp&   object_,                            \
                       const std::string& name_,                              \
                       int                width_ )                            \
{                                                                             \
    if( add_trace_check(name_) )                                              \
        traces.push_back( new vcd_ ## tp ## _trace( object_,                  \
                                                    name_,                    \
                                                    obtain_name(),            \
                                                    width_ ) );               \
}

DEFN_TRACE_METHOD_LONG_LONG(int64)
DEFN_TRACE_METHOD_LONG_LONG(uint64)

#undef DEFN_TRACE_METHOD_LONG_LONG

void
vcd_trace_file::trace( const unsigned&    object_,
                       const std::string& name_,
                       const char**       enum_literals_ )
{
    if( add_trace_check(name_) )
        traces.push_back( new vcd_enum_trace( object_,
                                              name_,
                                              obtain_name(),
                                              enum_literals_ ) );
}


void
vcd_trace_file::write_comment(const std::string& comment)
{
    if(!fp) open_fp();
    //no newline in comments allowed, as some viewers may crash
    std::fputs("$comment\n", fp);
    std::fputs(comment.c_str(), fp);
    std::fputs("\n$end\n\n", fp);
}

void
vcd_trace_file::cycle(bool this_is_a_delta_cycle)
{
    // Trace delta cycles only when enabled
    if (!delta_cycles() && this_is_a_delta_cycle) return;

    // Check for initialization
    if( initialize() )
        return;

    unit_type now_units_high, now_units_low;

    bool time_advanced = get_time_stamp(now_units_high, now_units_low);

    if (!has_low_units() && (now_units_low != 0)) {
        std::stringstream ss;
        ss << "\n\tCurrent kernel time is " << sc_time_stamp();
        ss << "\n\tVCD trace time unit is " << fs_unit_to_str(trace_unit_fs);
        ss << "\n\tUse 'tracefile->set_time_unit(double, sc_time_unit);' to increase the time resolution.";
        SC_REPORT_WARNING( SC_ID_TRACING_VCD_TIME_RESOLUTION_, ss.str().c_str() );
    }

    if (delta_cycles()) {

        if(this_is_a_delta_cycle) {
            static bool warned = false;
            if(!warned){
                SC_REPORT_INFO( SC_ID_TRACING_VCD_DELTA_CYCLE_
                , fs_unit_to_str(trace_unit_fs).c_str() );
                warned = true;
            }

            if (sc_delta_count_at_current_time() == 0) {
                if(!time_advanced) {
                    std::stringstream ss;
                    ss <<"\n\tThis can occur when delta cycle tracing is activated."
                       <<"\n\tSome delta cycles at " << sc_time_stamp() << " are not shown in vcd."
                       <<"\n\tUse 'tracefile->set_time_unit(double, sc_time_unit);' to increase the time resolution.";
                    SC_REPORT_WARNING( SC_ID_TRACING_REVERSED_TIME_, ss.str().c_str() );

                    return;
                }
            }
        }

        if (!this_is_a_delta_cycle) {
            if (time_advanced) {
                previous_time_units_high = now_units_high;
                previous_time_units_low = now_units_low;
            }
            // Value updates can't happen during timed notification
            // so it is safe to skip printing
            return;
        }
    }

    // Now do the actual printing
    bool time_printed = false;
    vcd_trace* const* const l_traces = &traces[0];
    for (int i = 0; i < (int)traces.size(); i++) {
        vcd_trace* t = l_traces[i];
        if(t->changed()) {
            if(!time_printed){
                print_time_stamp(now_units_high, now_units_low);

                time_printed = true;
            }

            // Write the variable
            t->write(fp);
            std::fputc('\n', fp);
        }
    }
    // Put another newline after all values are printed
    if(time_printed) std::fputc('\n', fp);
}

bool vcd_trace_file::get_time_stamp(sc_trace_file_base::unit_type &now_units_high,
                                    sc_trace_file_base::unit_type &now_units_low) const
{
    timestamp_in_trace_units(now_units_high, now_units_low);

    return ( (now_units_low > previous_time_units_low && now_units_high == previous_time_units_high)
            || now_units_high > previous_time_units_high);

}

void vcd_trace_file::print_time_stamp(sc_trace_file_base::unit_type now_units_high,
                                      sc_trace_file_base::unit_type now_units_low) const
{

    std::stringstream ss;
    if(has_low_units())
        ss << "#" << now_units_high << std::setfill('0') << std::setw(low_units_len()) << now_units_low;
    else
        ss << "#" << now_units_high;

    fputs(ss.str().c_str(), fp);
    fputc('\n', fp);
}


// Create a VCD name for a variable
std::string
vcd_trace_file::obtain_name()
{
    const char first_type_used = 'a';
    const int used_types_count = 'z' - 'a' + 1;
    int result;

    result = vcd_name_index;
    char char6 = static_cast<char>(vcd_name_index % used_types_count);

    result = result / used_types_count;
    char char5 = static_cast<char>(result % used_types_count);

    result = result / used_types_count;
    char char4 = static_cast<char>(result % used_types_count);

    result = result / used_types_count;
    char char3 = static_cast<char>(result % used_types_count);

    result = result / used_types_count;
    char char2 = static_cast<char>(result % used_types_count);

    char buf[20];
    std::snprintf(buf, sizeof(buf), "%c%c%c%c%c",
            char2 + first_type_used,
            char3 + first_type_used,
            char4 + first_type_used,
            char5 + first_type_used,
            char6 + first_type_used);
    vcd_name_index++;
    return std::string(buf);
}

vcd_trace_file::~vcd_trace_file()
{
    unit_type now_units_high, now_units_low;
    if (is_initialized() && get_time_stamp(now_units_high,now_units_low)) {
        print_time_stamp(now_units_high, now_units_low);
    }

    for( int i = 0; i < (int)traces.size(); i++ ) {
        vcd_trace* t = traces[i];
        delete t;
    }
}


// Functions specific to VCD tracing

static char
map_sc_logic_state_to_vcd_state(char in_char)
{
    char out_char;

    switch(in_char){
        case 'U':
        case 'X':
        case 'W':
        case 'D':
            out_char = 'x';
            break;
        case '0':
        case 'L':
            out_char = '0';
            break;
        case  '1':
        case  'H':
            out_char = '1';
            break;
        case  'Z':
            out_char = 'z';
            break;
        default:
            out_char = '?';
    }

    return out_char;
}




static
void
remove_vcd_name_problems(vcd_trace const* vcd, std::string& name)
{
    static bool warned = false;
    bool braces_removed = false;
    for (unsigned int i = 0; i< name.length(); i++) {
      if (name[i] == '[') {
	name[i] = '(';
	braces_removed = true;
      }
      else if (name[i] == ']') {
	name[i] = ')';
	braces_removed = true;
      }
    }

    if(braces_removed && !warned){
        std::stringstream ss;
        ss << vcd->name << ":\n"
            "\tTraced objects found with name containing [], which may be\n"
            "\tinterpreted by the waveform viewer in unexpected ways.\n"
            "\tSo the [] is automatically replaced by ().";

        SC_REPORT_WARNING( SC_ID_TRACING_OBJECT_NAME_FILTERED_
                         , ss.str().c_str() );
    }
}

// ----------------------------------------------------------------------------

SC_API sc_trace_file*
sc_create_vcd_trace_file(const char * name)
{
    sc_trace_file * tf = new vcd_trace_file(name);
    return tf;
}

SC_API void
sc_close_vcd_trace_file( sc_trace_file* tf )
{
    vcd_trace_file* vcd_tf = static_cast<vcd_trace_file*>(tf);
    delete vcd_tf;
}

} // namespace sc_core
