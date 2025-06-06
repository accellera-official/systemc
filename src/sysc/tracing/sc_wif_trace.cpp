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

  sc_wif_trace.cpp - Implementation of WIF tracing.

  Original Author - Abhijit Ghosh, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affliation, date and
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

/*****************************************************************************

   Instead of creating the binary WIF format, we create the ASCII
   WIF format which can be converted to the binary format using
   a2wif (utility that comes with VSS from Synopsys). This way,
   a user who does not have Synopsys VSS can still create WIF
   files, but they can only be viewed by users who have VSS.

 *****************************************************************************/


#include <cstdlib>
#include <limits>
#include <type_traits>
#include <vector>

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_ver.h"
#include "sysc/datatypes/bit/sc_bit.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/datatypes/bit/sc_lv_base.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/datatypes/int/sc_vector_utils.h"
#include "sysc/datatypes/int/sc_signed_inlines.h"
#include "sysc/datatypes/int/sc_signed_ops.h"
#include "sysc/datatypes/int/sc_unsigned_inlines.h"
#include "sysc/datatypes/fx/fx.h"
#include "sysc/tracing/sc_wif_trace.h"

#include <sstream>
#include <iomanip>

#if defined(_MSC_VER)
# pragma warning(disable:4309) // truncation of constant value
#endif

namespace sc_core {

using sc_dt::sc_max;
using sc_dt::sc_min;

// Forward declarations for functions that come later in the file
static char map_sc_logic_state_to_wif_state(char in_char);

const char* wif_names[wif_trace_file::WIF_LAST] = {"BIT","MVL","real"};


// ----------------------------------------------------------------------------
//  CLASS : wif_trace
//
//  Base class for WIF traces.
// ----------------------------------------------------------------------------

class wif_trace
{
public:

    wif_trace(const std::string& name_, const std::string& wif_name_);

    // Needs to be pure virtual as has to be defined by the particular
    // type being traced
    virtual void write(FILE* f) = 0;

    virtual void set_width();

    // Comparison function needs to be pure virtual too
    virtual bool changed() = 0;

    // Got to declare this virtual as this will be overwritten
    // by one base class
    virtual void print_variable_declaration_line(FILE* f);

    virtual ~wif_trace();

    const std::string name;     // Name of the variable
    const std::string wif_name; // Name of the variable in WIF file
    const char* wif_type;     // WIF data type
    int bit_width;
};


wif_trace::wif_trace(const std::string& name_,
	const std::string& wif_name_)
        : name(name_), wif_name(wif_name_), wif_type(0), bit_width(-1)
{
    /* Intentionally blank */
}

void
wif_trace::print_variable_declaration_line( FILE* f )
{
    if( bit_width < 0 )
    {
        std::stringstream ss;
        ss << "'" << name << "' has < 0 bits";
        SC_REPORT_ERROR( SC_ID_TRACING_OBJECT_IGNORED_
                       , ss.str().c_str() );
        return;
    }

    std::fprintf( f, "declare  %s   \"%s\"  %s  ",
                  wif_name.c_str(), name.c_str(), wif_type );

    if( bit_width > 0 ) {
        std::fprintf( f, "0 %d ", bit_width - 1 );
    }
    std::fprintf( f, "variable ;\n" );
    std::fprintf( f, "start_trace %s ;\n", wif_name.c_str() );
}

void
wif_trace::set_width()
{
    /* Intentionally Blank, should be defined for each type separately */
}

wif_trace::~wif_trace()
{
    /* Intentionally Blank */
}

// Classes for tracing individual data types

/*****************************************************************************/

template<typename T, bool Signed> struct wif_builtin_trace_attr;
template<typename T>  struct wif_builtin_trace_attr<T,true>
{
  using type = T;
  using utype = typename std::make_unsigned<type>::type;
  static const int max_width = sizeof(type) * BITS_PER_BYTE;

  unsigned rem_bits = 0U;

  void init(int bit_width)
    { rem_bits = max_width - bit_width; }
  bool overflow(const T& object) const
    { return (static_cast<type>(static_cast<utype>(object) << rem_bits) >> rem_bits) != object; }
};
template<typename T>  struct wif_builtin_trace_attr<T,false>
{
  using type = T;
  using utype = typename std::make_unsigned<type>::type;
  static const int max_width = sizeof(type) * BITS_PER_BYTE;

  T mask = ~type{};

  void init(int bit_width)
    { mask = ~(mask << bit_width); }
  bool overflow(const T& object) const
    { return (object & mask) != object; }
};

template<typename T, bool Signed = std::is_signed<T>::value>
class wif_builtin_trace
  : public wif_trace
  , protected wif_builtin_trace_attr<T, Signed>
{
    static_assert(std::is_integral<T>::value, "integral type required");
    using attr = wif_builtin_trace_attr<T, Signed>;
public:
    using type  = typename attr::type;
    using utype = typename attr::utype;
    using attr::max_width;

    wif_builtin_trace(const type& object_,
                      const std::string& name_,
                      const std::string& wif_name_,
                      int   width_);

    bool changed() { return object != old_value; }
    void write(FILE* f);

protected:
    const type& object;
    type old_value;
};


template<typename T, bool Signed>
wif_builtin_trace<T, Signed>::wif_builtin_trace( const type& object_,
                                                 const std::string& name_,
                                                 const std::string& wif_name_,
                                                 int width_ )
  : wif_trace(name_, wif_name_), object(object_), old_value(object_)
{
    bit_width = sc_min(+max_width, sc_max(width_, 0));
    if (bit_width < max_width)
       attr::init(bit_width);
    wif_type = "BIT";
}


template<typename T, bool Signed>
void wif_builtin_trace<T, Signed>::write(FILE* f)
{
    char rawdata[max_width + /*\0*/ 1], *rawdata_ptr = rawdata;

    if (attr::overflow(object))
    {
        for (int bit_index = 0; bit_index < bit_width; ++bit_index) {
            *rawdata_ptr++ = '0';
        }
    }
    else
    {
        auto bit_mask = utype{1} << (bit_width-1);
        for (int bit_index = 0; bit_index < bit_width; ++bit_index) {
            *rawdata_ptr++ = (object & bit_mask)? '1' : '0';
            bit_mask = bit_mask >> 1;
        }
    }
    *rawdata_ptr = '\0';
    std::fprintf(f, "assign %s \"%s\" ;\n", wif_name.c_str(), rawdata);
    old_value = object;
}

/*****************************************************************************/

class wif_bool_trace
: public wif_trace
{
public:

    wif_bool_trace( const bool& object_,
		    const std::string& name_,
		    const std::string& wif_name_ );
    void write( FILE* f );
    bool changed();

protected:

    const bool& object;
    bool        old_value;
};

wif_bool_trace::wif_bool_trace( const bool& object_,
				const std::string& name_,
				const std::string& wif_name_ )
: wif_trace( name_, wif_name_ ), object( object_ ), old_value( object_ )
{
    bit_width = 0;
    wif_type = "BIT";
}

bool
wif_bool_trace::changed()
{
    return object != old_value;
}

void
wif_bool_trace::write( FILE* f )
{
    std::fprintf( f, "assign %s \'%c\' ;\n", wif_name.c_str(), "01"[object] );
    old_value = object;
}

//*****************************************************************************

class wif_sc_bit_trace : public wif_trace {
public:
    wif_sc_bit_trace(const sc_dt::sc_bit& object_,
                     const std::string& name_,
                     const std::string& wif_name_);
    void write(FILE* f);
    bool changed();

protected:
    const sc_dt::sc_bit& object;
    sc_dt::sc_bit old_value;
};

wif_sc_bit_trace::wif_sc_bit_trace(const sc_dt::sc_bit& object_,
				   const std::string& name_,
				   const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_)
{
    bit_width = 0;
    wif_type = "BIT";
}

bool wif_sc_bit_trace::changed()
{
    return object != old_value;
}

void wif_sc_bit_trace::write(FILE* f)
{
    std::fprintf( f, "assign %s \'%c\' ;\n", wif_name.c_str(), "01"[object] );
    old_value = object;
}

/*****************************************************************************/

class wif_sc_logic_trace: public wif_trace {
public:
    wif_sc_logic_trace(const sc_dt::sc_logic& object_,
		       const std::string& name_,
		       const std::string& wif_name_);
    void write(FILE* f);
    bool changed();

protected:
    const sc_dt::sc_logic& object;
    sc_dt::sc_logic old_value;
};


wif_sc_logic_trace::wif_sc_logic_trace(const sc_dt::sc_logic& object_,
				       const std::string& name_,
				       const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_)
{
    bit_width = 0;
    wif_type = "MVL";
}


bool wif_sc_logic_trace::changed()
{
    return object != old_value;
}


void wif_sc_logic_trace::write(FILE* f)
{
    char wif_char = map_sc_logic_state_to_wif_state(object.to_char());
    std::fprintf(f, "assign %s \'%c\' ;\n", wif_name.c_str(), wif_char);
    old_value = object;
}


/*****************************************************************************/

class wif_sc_unsigned_trace: public wif_trace {
public:
    wif_sc_unsigned_trace(const sc_dt::sc_unsigned& object_,
			  const std::string& name_,
			  const std::string& wif_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_unsigned& object;
    sc_dt::sc_unsigned old_value;
    std::vector<char> rawdata;
};


wif_sc_unsigned_trace::wif_sc_unsigned_trace(const sc_dt::sc_unsigned& object_,
					     const std::string& name_,
					     const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_.length())
{
    old_value = object;
    wif_type = "BIT";
}

bool wif_sc_unsigned_trace::changed()
{
    return object != old_value;
}

void wif_sc_unsigned_trace::write(FILE* f)
{
    char *rawdata_ptr  = rawdata.data();
    for(int bit_index = bit_width - 1; bit_index >= 0; --bit_index) {
        *rawdata_ptr++ = "01"[object[bit_index].to_bool()];
    }
    *rawdata_ptr = '\0';
    std::fprintf(f, "assign %s \"%s\" ;\n", wif_name.c_str(), rawdata.data());
    old_value = object;
}

void wif_sc_unsigned_trace::set_width()
{
    bit_width = object.length();
    rawdata.resize(bit_width + 1); // include trailing \0
}


/*****************************************************************************/

class wif_sc_signed_trace: public wif_trace {
public:
    wif_sc_signed_trace(const sc_dt::sc_signed& object_,
			const std::string& name_,
			const std::string& wif_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_signed& object;
    sc_dt::sc_signed old_value;
    std::vector<char> rawdata;
};


wif_sc_signed_trace::wif_sc_signed_trace(const sc_dt::sc_signed& object_,
					 const std::string& name_,
					 const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_.length())
{
    old_value = object;
    wif_type = "BIT";
}

bool wif_sc_signed_trace::changed()
{
    return object != old_value;
}

void wif_sc_signed_trace::write(FILE* f)
{
    char *rawdata_ptr  = rawdata.data();
    for(int bit_index = bit_width - 1; bit_index >= 0; --bit_index) {
        *rawdata_ptr++ = "01"[object[bit_index].to_bool()];
    }
    *rawdata_ptr = '\0';
    std::fprintf(f, "assign %s \"%s\" ;\n", wif_name.c_str(), rawdata.data());
    old_value = object;
}

void wif_sc_signed_trace::set_width()
{
    bit_width = object.length();
    rawdata.resize(bit_width + 1); // include trailing \0
}

/*****************************************************************************/

class wif_sc_uint_base_trace: public wif_trace
{
    static const int max_width = sizeof(sc_dt::uint64) * BITS_PER_BYTE;
public:
    wif_sc_uint_base_trace(const sc_dt::sc_uint_base& object_,
			   const std::string& name_,
			   const std::string& wif_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_uint_base& object;
    sc_dt::sc_uint_base old_value;
};


wif_sc_uint_base_trace::wif_sc_uint_base_trace(
                                          const sc_dt::sc_uint_base& object_,
					  const std::string& name_,
					  const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_.length())
{
    old_value = object;
    wif_type = "BIT";
}

bool wif_sc_uint_base_trace::changed()
{
    return object != old_value;
}

void wif_sc_uint_base_trace::write(FILE* f)
{
    char rawdata[max_width + /*\0*/ 1], *rawdata_ptr = rawdata;
    for(int bit_index = bit_width - 1; bit_index >= 0; --bit_index) {
        *rawdata_ptr++ = "01"[object[bit_index].to_bool()];
    }
    *rawdata_ptr = '\0';
    std::fprintf(f, "assign %s \"%s\" ;\n", wif_name.c_str(), rawdata);
    old_value = object;
}

void wif_sc_uint_base_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class wif_sc_int_base_trace: public wif_trace
{
    static const int max_width = sizeof(sc_dt::int64) * BITS_PER_BYTE;
public:
    wif_sc_int_base_trace(const sc_dt::sc_int_base& object_,
			  const std::string& name_,
			  const std::string& wif_name_);
    void write(FILE* f);
    bool changed();
    void set_width();

protected:
    const sc_dt::sc_int_base& object;
    sc_dt::sc_int_base old_value;
};


wif_sc_int_base_trace::wif_sc_int_base_trace(const sc_dt::sc_int_base& object_,
					     const std::string& name_,
					     const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_.length())
{
    old_value = object;
    wif_type = "BIT";
}

bool wif_sc_int_base_trace::changed()
{
    return object != old_value;
}

void wif_sc_int_base_trace::write(FILE* f)
{
    char rawdata[max_width + /*\0*/ 1], *rawdata_ptr = rawdata;
    for(int bit_index = bit_width - 1; bit_index >= 0; --bit_index) {
        *rawdata_ptr++ = "01"[object[bit_index].to_bool()];
    }
    *rawdata_ptr = '\0';
    std::fprintf(f, "assign %s \"%s\" ;\n", wif_name.c_str(), rawdata);
    old_value = object;
}

void wif_sc_int_base_trace::set_width()
{
    bit_width = object.length();
}


/*****************************************************************************/

class wif_sc_fxval_trace: public wif_trace
{
public:

    wif_sc_fxval_trace( const sc_dt::sc_fxval& object_,
			const std::string& name_,
			const std::string& wif_name_ );
    void write( FILE* f );
    bool changed();

protected:

    const sc_dt::sc_fxval& object;
    sc_dt::sc_fxval old_value;

};

wif_sc_fxval_trace::wif_sc_fxval_trace( const sc_dt::sc_fxval& object_,
				        const std::string& name_,
					const std::string& wif_name_ )
: wif_trace( name_, wif_name_ ), object( object_ ), old_value( object_ )
{
    bit_width = 0;
    wif_type = "real";
}

bool
wif_sc_fxval_trace::changed()
{
    return object != old_value;
}

void
wif_sc_fxval_trace::write( FILE* f )
{
    std::fprintf( f, "assign  %s %f ; \n", wif_name.c_str(), object.to_double() );
    old_value = object;
}

/*****************************************************************************/

class wif_sc_fxval_fast_trace: public wif_trace
{
public:

    wif_sc_fxval_fast_trace( const sc_dt::sc_fxval_fast& object_,
			     const std::string& name_,
			     const std::string& wif_name_ );
    void write( FILE* f );
    bool changed();

protected:

    const sc_dt::sc_fxval_fast& object;
    sc_dt::sc_fxval_fast old_value;

};

wif_sc_fxval_fast_trace::wif_sc_fxval_fast_trace(
                                const sc_dt::sc_fxval_fast& object_,
				const std::string& name_,
				const std::string& wif_name_ )
: wif_trace(name_, wif_name_), object( object_ ), old_value( object_ )
{
    bit_width = 0;
    wif_type = "real";
}

bool
wif_sc_fxval_fast_trace::changed()
{
    return object != old_value;
}

void
wif_sc_fxval_fast_trace::write( FILE* f )
{
    std::fprintf( f, "assign  %s %f ; \n", wif_name.c_str(), object.to_double() );
    old_value = object;
}

/*****************************************************************************/

class wif_sc_fxnum_trace: public wif_trace
{
public:

    wif_sc_fxnum_trace( const sc_dt::sc_fxnum& object_,
			const std::string& name_,
			const std::string& wif_name_ );
    void write( FILE* f );
    bool changed();
    void set_width();

protected:

    const sc_dt::sc_fxnum& object;
    sc_dt::sc_fxnum old_value;
    std::vector<char> rawdata;

};

wif_sc_fxnum_trace::wif_sc_fxnum_trace( const sc_dt::sc_fxnum& object_,
				        const std::string& name_,
					const std::string& wif_name_ )
: wif_trace( name_, wif_name_ ),
  object( object_ ),
  old_value( object_.m_params.type_params(),
	     object_.m_params.enc(),
	     object_.m_params.cast_switch(),
	     0 )
{
    old_value = object;
    wif_type = "BIT";
}

bool
wif_sc_fxnum_trace::changed()
{
    return object != old_value;
}

void
wif_sc_fxnum_trace::write( FILE* f )
{
    char *rawdata_ptr  = rawdata.data();
    for(int bit_index = bit_width - 1; bit_index >= 0; --bit_index) {
        *rawdata_ptr++ = "01"[object[bit_index]];
    }
    std::fprintf( f, "assign %s \"%s\" ;\n", wif_name.c_str(), rawdata.data());
    old_value = object;
}

void
wif_sc_fxnum_trace::set_width()
{
    bit_width = object.wl();
    rawdata.resize(bit_width + 1); // include trailing \0
}

/*****************************************************************************/

class wif_sc_fxnum_fast_trace: public wif_trace
{
public:

    wif_sc_fxnum_fast_trace( const sc_dt::sc_fxnum_fast& object_,
			     const std::string& name_,
			     const std::string& wif_name_ );
    void write( FILE* f );
    bool changed();
    void set_width();

protected:

    const sc_dt::sc_fxnum_fast& object;
    sc_dt::sc_fxnum_fast old_value;
    std::vector<char> rawdata;

};

wif_sc_fxnum_fast_trace::wif_sc_fxnum_fast_trace(
				const sc_dt::sc_fxnum_fast& object_,
				const std::string& name_,
				const std::string& wif_name_ )
: wif_trace( name_, wif_name_ ),
  object( object_ ),
  old_value( object_.m_params.type_params(),
	     object_.m_params.enc(),
	     object_.m_params.cast_switch(),
	     0 )
{
    old_value = object;
    wif_type = "BIT";
}

bool
wif_sc_fxnum_fast_trace::changed()
{
    return object != old_value;
}

void
wif_sc_fxnum_fast_trace::write( FILE* f )
{
    char *rawdata_ptr  = rawdata.data();
    for(int bit_index = bit_width - 1; bit_index >= 0; --bit_index) {
        *rawdata_ptr++ = "01"[object[bit_index]];
    }
    *rawdata_ptr = '\0';
    std::fprintf( f, "assign %s \"%s\" ;\n", wif_name.c_str(), rawdata.data());
    old_value = object;
}

void
wif_sc_fxnum_fast_trace::set_width()
{
    bit_width = object.wl();
    rawdata.resize(bit_width + 1); // incl trailing 0
}

/*****************************************************************************/

class wif_float_trace: public wif_trace {
public:
    wif_float_trace(const float& object_,
		    const std::string& name_,
		    const std::string& wif_name_);
    void write(FILE* f);
    bool changed();

protected:
    const float& object;
    float old_value;
};

wif_float_trace::wif_float_trace(const float& object_,
				 const std::string& name_,
				 const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_)
{
    bit_width = 0;
    wif_type = "real";
}

bool wif_float_trace::changed()
{
    return object != old_value;
}

void wif_float_trace::write(FILE* f)
{
    std::fprintf(f,"assign  %s %f ; \n", wif_name.c_str(), object);
    old_value = object;
}

/*****************************************************************************/

class wif_double_trace: public wif_trace {
public:
    wif_double_trace(const double& object_,
		     const std::string& name_,
		     const std::string& wif_name_);
    void write(FILE* f);
    bool changed();

protected:
    const double& object;
    double old_value;
};

wif_double_trace::wif_double_trace(const double& object_,
				   const std::string& name_,
				   const std::string& wif_name_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_)
{
    bit_width = 0;
    wif_type = "real";
}

bool wif_double_trace::changed()
{
    return object != old_value;
}

void wif_double_trace::write(FILE* f)
{
    std::fprintf(f,"assign  %s %f ; \n", wif_name.c_str(), object);
    old_value = object;
}


/*****************************************************************************/

class wif_enum_trace : public wif_trace {
public:
    wif_enum_trace(const unsigned& object_,
		   const std::string& name_,
		   const std::string& wif_name_,
		   const char** enum_literals);
    void write(FILE* f);
    bool changed();
    // Hides the definition of the same (virtual) function in wif_trace
    void print_variable_declaration_line(FILE* f);

protected:
    const unsigned& object;
    unsigned old_value;

    const char** literals;
    unsigned nliterals;
    std::string type_name;

    ~wif_enum_trace();
};


wif_enum_trace::wif_enum_trace(const unsigned& object_,
			       const std::string& name_,
			       const std::string& wif_name_,
			       const char** enum_literals_)
: wif_trace(name_, wif_name_), object(object_), old_value(object_),
  literals(enum_literals_), nliterals(0), type_name(name_ + "__type__")
{
    // find number of enumeration literals - counting loop
    for (nliterals = 0; enum_literals_[nliterals]; nliterals++) continue;

    bit_width = 0;
    wif_type = type_name.c_str();
}

void wif_enum_trace::print_variable_declaration_line(FILE* f)
{
    std::fprintf(f, "type scalar \"%s\" enum ", wif_type);

    for (unsigned i = 0; i < nliterals; i++)
      std::fprintf(f, "\"%s\", ", literals[i]);
    std::fprintf(f, "\"SC_WIF_UNDEF\" ;\n");

    std::fprintf(f, "declare  %s   \"%s\"  \"%s\" ",
	    wif_name.c_str(), name.c_str(), wif_type);
    std::fprintf(f, "variable ;\n");
    std::fprintf(f, "start_trace %s ;\n", wif_name.c_str());
}

bool wif_enum_trace::changed()
{
    return object != old_value;
}

void wif_enum_trace::write(FILE* f)
{
    static bool warning_issued = false;
    const char* lit;

    if (object >= nliterals) { // Note unsigned value is always greater than 0
        if (!warning_issued) {
            SC_REPORT_WARNING( SC_ID_TRACING_INVALID_ENUM_VALUE_
                             , name.c_str() );
            warning_issued = true;
        }
        lit = "SC_WIF_UNDEF";
    }
    else
    {
        lit = literals[object];
    }
    std::fprintf( f, "assign %s \"%s\" ;\n", wif_name.c_str(), lit );
    old_value = object;
}

wif_enum_trace::~wif_enum_trace()
{
    /* Intentionally blank */
}


template <class T>
class wif_T_trace
: public wif_trace
{
public:

    wif_T_trace( const T& object_,
		 const std::string& name_,
		 const std::string& wif_name_,
		 wif_trace_file::wif_enum type_ )
    : wif_trace( name_, wif_name_),
      object( object_ ),
      old_value( object_ )
    {  wif_type = wif_names[type_]; }

    void write( FILE* f )
    {
       std::fprintf( f,
		"assign %s \"%s\" ;\n",
		wif_name.c_str(),
		object.to_string().c_str() );
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

typedef wif_T_trace<sc_dt::sc_bv_base> wif_sc_bv_trace;
typedef wif_T_trace<sc_dt::sc_lv_base> wif_sc_lv_trace;


//***********************************************************************
//           wif_trace_file functions
//***********************************************************************


wif_trace_file::wif_trace_file(const char * name)
  : sc_trace_file_base( name, "awif" )
  , wif_name_index(0)
  , previous_units_low(0)
  , previous_units_high(0)
  , traces()
{}


void wif_trace_file::do_initialize()
{
    char buf[2000];

    // init
    std::fprintf(fp, "init ;\n\n");

    //timescale:
    std::snprintf(buf, sizeof(buf), "%d", static_cast<int>(log10(static_cast<double>(trace_unit_fs))));
    std::fprintf(fp,"header  %s \"%s\" ;\n\n", buf, sc_version());

    std::fprintf(fp, "comment \"ASCII WIF file produced on date:  %s\" ;\n"
                , localtime_string().c_str());

    //version:
    std::fprintf(fp, "comment \"Created by %s\" ;\n", sc_version());
    //conversion info
    std::fprintf(fp, "comment \"Convert this file to binary WIF format using a2wif\" ;\n\n");

    // Define the two types we need to represent bool and sc_logic
    std::fprintf(fp, "type scalar \"BIT\" enum '0', '1' ;\n");
    std::fprintf(fp, "type scalar \"MVL\" enum '0', '1', 'X', 'Z', '?' ;\n");
    std::fprintf(fp, "\n");

    //variable definitions:
    int i;
    for (i = 0; i < (int)traces.size(); i++) {
        wif_trace* t = traces[i];
        t->set_width(); //needed for all vectors
        t->print_variable_declaration_line(fp);
    }

    std::stringstream ss;

    timestamp_in_trace_units(previous_units_high, previous_units_low);

    ss << "All initial values are dumped below at time "
       << sc_time_stamp().to_seconds() <<" sec = ";
    if(has_low_units())
        ss << previous_units_high << std::setfill('0') << std::setw(low_units_len()) << previous_units_low;
    else
        ss << previous_units_high;
    ss << " timescale units.";

    write_comment(ss.str());

    for (i = 0; i < (int)traces.size(); i++) {
        wif_trace* t = traces[i];
        t->write(fp);
    }
    std::fprintf(fp, "\n");
}

void wif_trace_file::trace( sc_trace_file* ) const {
    SC_REPORT_ERROR( sc_core::SC_ID_INTERNAL_ERROR_
                   , "invalid call to wif_trace_file::trace(sc_trace_file*)" );
}

// ----------------------------------------------------------------------------

void wif_trace_file::trace(const sc_event&, const std::string& name) {
    std::stringstream msg;
    msg << "sc_events are not supported by WIF trace: " << name;
    SC_REPORT_ERROR(SC_ID_TRACING_OBJECT_IGNORED_, msg.str().c_str() );
}

void wif_trace_file::trace(const sc_time&, const std::string& name) {
    std::stringstream msg;
    msg << "sc_time is not supported by WIF trace: " << name;
    SC_REPORT_ERROR(SC_ID_TRACING_OBJECT_IGNORED_, msg.str().c_str() );
}

#define DEFN_TRACE_METHOD(tp)                                                 \
void                                                                          \
wif_trace_file::trace( const tp& object_, const std::string& name_ )          \
{                                                                             \
    if( add_trace_check(name_) )                                              \
        traces.push_back( new wif_ ## tp ## _trace( object_,                  \
                                                    name_,                    \
                                                    obtain_name() ) );        \
}

DEFN_TRACE_METHOD(bool)
DEFN_TRACE_METHOD(float)
DEFN_TRACE_METHOD(double)

#undef DEFN_TRACE_METHOD
#define DEFN_TRACE_METHOD(tp)                                                 \
void                                                                          \
wif_trace_file::trace(const sc_dt::tp& object_, const std::string& name_)     \
{                                                                             \
    if( add_trace_check(name_) )                                              \
        traces.push_back( new wif_ ## tp ## _trace( object_,                  \
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


#define DEFN_TRACE_METHOD_INTEGRAL(tp)                              \
void                                                                \
wif_trace_file::trace( const tp&          object_,                  \
                       const std::string& name_,                    \
                       int                width_ )                  \
{                                                                   \
    if( add_trace_check(name_) )                                    \
        traces.push_back( new wif_builtin_trace<tp>( object_,       \
                                                     name_,         \
                                                     obtain_name(), \
                                                     width_ ) );    \
}

DEFN_TRACE_METHOD_INTEGRAL(char)
DEFN_TRACE_METHOD_INTEGRAL(short)
DEFN_TRACE_METHOD_INTEGRAL(int)
DEFN_TRACE_METHOD_INTEGRAL(long)
DEFN_TRACE_METHOD_INTEGRAL(sc_dt::int64)

DEFN_TRACE_METHOD_INTEGRAL(unsigned char)
DEFN_TRACE_METHOD_INTEGRAL(unsigned short)
DEFN_TRACE_METHOD_INTEGRAL(unsigned int)
DEFN_TRACE_METHOD_INTEGRAL(unsigned long)
DEFN_TRACE_METHOD_INTEGRAL(sc_dt::uint64)

#undef DEFN_TRACE_METHOD_INTEGRAL


void
wif_trace_file::trace( const unsigned& object_,
		       const std::string& name_,
		       const char** enum_literals_ )
{
    if( add_trace_check(name_) )
        traces.push_back( new wif_enum_trace( object_,
                                              name_,
                                              obtain_name(),
                                              enum_literals_ ) );
}

void
wif_trace_file::trace( const sc_dt::sc_bv_base& object_,
    const std::string& name_ )
{
   traceT( object_, name_, WIF_BIT );
}

void
wif_trace_file::trace( const sc_dt::sc_lv_base& object_,
    const std::string& name_ )
{
   traceT( object_, name_, WIF_MVL );
}


void
wif_trace_file::write_comment(const std::string& comment)
{
    if(!fp) open_fp();
    //no newline in comments allowed
    std::fprintf(fp, "comment \"%s\" ;\n", comment.c_str());
}


void
wif_trace_file::cycle(bool this_is_a_delta_cycle)
{
    unit_type now_units_high, now_units_low;

    // Trace delta cycles only when enabled
    if (!delta_cycles() && this_is_a_delta_cycle) return;
    // When delta cycles enabled, value changes happen only during delta cycles
    if (delta_cycles() && !this_is_a_delta_cycle) return;

    // Check for initialization
    if( initialize() ) {
        return;
    };

    timestamp_in_trace_units(now_units_high, now_units_low);

    unit_type delta_units_high, delta_units_low;
    bool time_advanced = true;
    if (now_units_low < previous_units_low) {
        unit_type max_low_units = kernel_unit_fs / trace_unit_fs;
        delta_units_low = max_low_units - previous_units_low + now_units_low;

        if (now_units_high <= previous_units_high)
            time_advanced = false;

        delta_units_high = now_units_high - 1 - previous_units_high;
    } else {
        delta_units_low = now_units_low - previous_units_low;

        if (now_units_high < previous_units_high || ((now_units_high == previous_units_high) && delta_units_low == 0) )
            time_advanced = false;

        delta_units_high = now_units_high - previous_units_high;
    }

    if (!time_advanced) {
        std::stringstream ss;
        ss <<"\n\tThis can occur when delta cycle tracing is activated."
           <<"\n\tSome delta cycles at " << sc_time_stamp() << " are not shown in trace file."
           <<"\n\tUse 'tracefile->set_time_unit(double, sc_time_unit);' to increase the time resolution.";
        SC_REPORT_WARNING( SC_ID_TRACING_REVERSED_TIME_, ss.str().c_str() );
        return;
    }

    bool time_printed = false;
    for (size_t i = 0; i < traces.size(); i++) {
        wif_trace* t = traces[i];
        if(t->changed()){
            if(time_printed == false) {

                std::stringstream ss;

                if(has_low_units())
                    ss << "delta_time " << delta_units_high << std::setfill('0')
                       << std::setw(low_units_len()) << delta_units_low << " ;\n";
                else
                    ss << "delta_time " << delta_units_high <<" ;\n";

                std::fputs(ss.str().c_str(), fp);

                time_printed = true;
            }

            // Write the variable
            t->write(fp);
        }
    }

    if(time_printed) {
        std::fprintf(fp, "\n");     // Put another newline
        // We update previous_time_units only when we print time because
        // this field stores the previous time that was printed, not the
        // previous time this function was called
        previous_units_high = now_units_high;
        previous_units_low = now_units_low;
    }
}

#if 0
void
wif_trace_file::create_wif_name(std::string* ptr_to_str)
{
  obtain_name().swap(*ptr_to_str);
}
#endif

// Create a WIF name for a variable
std::string
wif_trace_file::obtain_name()
{
    char buf[32];
    std::snprintf( buf, sizeof(buf), "O%d", wif_name_index ++ );
    return buf;
}

wif_trace_file::~wif_trace_file()
{
    for( int i = 0; i < (int)traces.size(); i++ ) {
        wif_trace* t = traces[i];
        delete t;
    }
}

// Map sc_logic values to values understandable by WIF
static char
map_sc_logic_state_to_wif_state(char in_char)
{
    char out_char;

    switch(in_char){
        case 'U':
        case 'X':
        case 'W':
        case 'D':
            out_char = 'X';
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
            out_char = 'Z';
            break;
        default:
            out_char = '?';
    }
    return out_char;
}

// ----------------------------------------------------------------------------

// Create the trace file
SC_API sc_trace_file*
sc_create_wif_trace_file(const char * name)
{
    sc_trace_file *tf = new wif_trace_file(name);
    return tf;
}

SC_API void
sc_close_wif_trace_file( sc_trace_file* tf )
{
    wif_trace_file* wif_tf = static_cast<wif_trace_file*>(tf);
    delete wif_tf;
}

} // namespace sc_core
