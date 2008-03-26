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

  sc_export.h -- Base classes of all export classes.

  Original Author: Andy Goodrich, Forte Design Systems
                   Bishnupriya Bhattacharya, Cadence Design Systems

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_EXPORT_H
#define SC_EXPORT_H
#include <typeinfo>

#include "sysc/communication/sc_communication_ids.h"
#include "sysc/communication/sc_interface.h"
#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_simcontext.h"

namespace sc_core {

//=============================================================================
//  CLASS : sc_export_base
//
//  Abstract base class for class sc_export<IF>.
//=============================================================================

class sc_export_base : public sc_object
{
    friend class sc_export_registry;
public:

    // typedefs
    
    typedef sc_export_base this_type;

public:

    virtual       sc_interface* get_interface() = 0;
    virtual       const sc_interface* get_interface() const = 0;

protected:
    
    // constructors

    sc_export_base();
    sc_export_base(const char* name);

    // destructor

    virtual ~sc_export_base();

protected:

    // called when construction is done 
    virtual void before_end_of_elaboration();

    // called when elaboration is done (does nothing by default)
    virtual void end_of_elaboration();

    // called before simulation starts (does nothing by default)
    virtual void start_of_simulation();

    // called after simulation ends (does nothing)
    virtual void end_of_simulation();

    virtual const char* if_typename() const = 0;

private:

    // disabled
    sc_export_base(const this_type&);
    this_type& operator = (const this_type& );

};

//=============================================================================
//  CLASS : sc_export
//
//  Generic export class for other export classes. This
//  class provides a binding point for access to an interface.
//=============================================================================
template<class IF>
class sc_export : public sc_export_base
{
    typedef sc_export<IF> this_type;

public: // constructors:
    sc_export() : sc_export_base()
    {
	m_interface_p = 0;
    }

    sc_export( const char* name_ ) : sc_export_base(name_)
    {
	m_interface_p = 0;
    }

    sc_export( IF& interface_ ) : sc_export_base()
    {
	m_interface_p = &interface_;
    }

    sc_export( const char* name_, IF& interface_ ) : sc_export_base(name_)
    {
	m_interface_p = &interface_;
    }

    explicit sc_export( this_type& child_ ) : sc_export_base()
    {
	m_interface_p = child_.m_interface_p;
    }

public: // destructor:
    virtual ~sc_export() 
    {
    }

public: // interface access:

    virtual sc_interface* get_interface() 
    {
	return m_interface_p;
    }

    virtual const sc_interface* get_interface() const
    {
        return m_interface_p;
    }

    const IF* operator -> () const {
        if ( m_interface_p == 0 )
        {
            SC_REPORT_ERROR(SC_ID_SC_EXPORT_HAS_NO_INTERFACE_,name());
        }
        return m_interface_p;
    }

    IF* operator -> () {
        if ( m_interface_p == 0 )
        {
            SC_REPORT_ERROR(SC_ID_SC_EXPORT_HAS_NO_INTERFACE_,name());
        }
        return m_interface_p;
    }

    operator IF& ()
    {
	if ( m_interface_p == 0 )
	{
	    SC_REPORT_ERROR(SC_ID_SC_EXPORT_HAS_NO_INTERFACE_,name());
	}
	return *m_interface_p;
    }

public: // binding:
    void bind( IF& interface_ )
    {
    	if ( m_interface_p )
	{
	    SC_REPORT_ERROR(SC_ID_SC_EXPORT_ALREADY_BOUND_,name());
	}
	else
	{
	    m_interface_p = &interface_;
	}
    }

    void operator () ( IF& interface_ )
    {
    	if ( m_interface_p )
	{
	    SC_REPORT_ERROR(SC_ID_SC_EXPORT_ALREADY_BOUND_,name());
	}
	else
	{
	    m_interface_p = &interface_;
	}
    }

public: // identification:
    virtual const char* kind() const { return "sc_export"; }

protected:
  const char* if_typename() const {
    return typeid( IF ).name();
  }

private: // disabled
    sc_export( const this_type& );
    this_type& operator = ( const this_type& );

protected: // data fields:
    IF* m_interface_p;		// Interface this port provides.
};

// ----------------------------------------------------------------------------
//  CLASS : sc_export_registry
//
//  Registry for all exports.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

class sc_export_registry
{
    friend class sc_simcontext;

public:

    void insert( sc_export_base* );
    void remove( sc_export_base* );

    int size() const
        { return m_export_vec.size(); }

private:

    // constructor
    explicit sc_export_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_export_registry();

    // called when construction is done
    void construction_done();

    // called when elaboration is done
    void elaboration_done();

    // called before simulation starts
    void start_simulation();

    // called after simulation ends
    void simulation_done();

private:

    sc_simcontext*              m_simc;
    sc_pvector<sc_export_base*>   	m_export_vec;

private:

    // disabled
    sc_export_registry();
    sc_export_registry( const sc_export_registry& );
    sc_export_registry& operator = ( const sc_export_registry& );
};

} // namespace sc_core

#endif

// Taf!
