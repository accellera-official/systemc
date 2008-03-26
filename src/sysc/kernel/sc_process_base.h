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

  sc_process_base.h -- Dynamic process support definitions.

  Original Authors: Andy Goodrich, Forte Design Systems, 17 June 2003
                    Stuart Swan, Cadence,
                    Bishnupriya Bhattacharya, Cadence Design Systems,
                    25 August, 2003
               

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_PROCESS_BASE_H
#define SC_PROCESS_BASE_H

#include "sysc/kernel/sc_process_host.h"
#include "sysc/utils/sc_vector.h"
#include "sysc/communication/sc_export.h"

namespace sc_core {

class sc_event;
class sc_port_base;
class sc_interface;
class sc_event_finder;


class sc_process_base;

//=============================================================================
// CLASS sc_spawn_options
//
//=============================================================================
class sc_spawn_options {
    friend class sc_process_base;
  public:
    sc_spawn_options() :                  
        m_dont_initialize(false), m_spawn_method(false), m_stack_size(0) 
        { }

    void spawn_method()                 { m_spawn_method = true; }
    void dont_initialize()              { m_dont_initialize = true; }
    void set_stack_size(int stack_size) { m_stack_size = stack_size; }
    void set_sensitivity(const sc_event* event) 
        { m_sensitive_events.push_back(event); }
    void set_sensitivity(sc_port_base* port_base)
        { m_sensitive_port_bases.push_back(port_base); }
    void set_sensitivity(sc_interface* interface_p) 
        { m_sensitive_interfaces.push_back(interface_p); }
    void set_sensitivity(sc_export_base* export_base) 
        { m_sensitive_interfaces.push_back(export_base->get_interface()); }
    void set_sensitivity(sc_event_finder* event_finder) 
        { m_sensitive_event_finders.push_back(event_finder); }

  private:
  	sc_spawn_options( const sc_spawn_options& );
  	const sc_spawn_options& operator = ( const sc_spawn_options& );

  protected:
    bool                         m_dont_initialize;         
    bool                         m_spawn_method;            
    int                          m_stack_size;           // Thread stack size.
    sc_pvector<const sc_event*>  m_sensitive_events;
    sc_pvector<sc_event_finder*> m_sensitive_event_finders; 
    sc_pvector<sc_interface*>    m_sensitive_interfaces;
    sc_pvector<sc_port_base*>    m_sensitive_port_bases;
};

//==============================================================================
// CLASS sc_process_base - USER INITIATED DYNAMIC PROCESS SUPPORT:
//
// This class implements the base class for a threaded process_base process 
// whose semantics are provided by the true virtual method semantics(). 
// Classes derived from this one will provide a version of semantics which 
// implements the desired semantics. See the sc_spawn_xxx classes below.
//
// Notes:
//   (1) Object instances of this class maintain a reference count of 
//       outstanding handles. When the handle count goes to zero the 
//       object will be deleted. 
//   (2) Descriptions of the methods and operators in this class appear with
//       their implementations.
//==============================================================================
class sc_process_base : public sc_process_host, public sc_process_monitor {
    friend class sc_process_handle; // Allow handles to modify ref. count.
  public:
    sc_process_base( const char* name, const sc_spawn_options* opt_p=0 );
    virtual ~sc_process_base();
    inline operator sc_method_handle ();
    inline operator sc_thread_handle ();
    void wait();

  private:
    inline void reference_decrement();
    inline void reference_increment();

  protected:
    virtual void semantics()=0;
    virtual void signal( sc_thread_handle thread_p, int type );

  protected:
    sc_process_base* m_delete_p;     // Next entry on deletion queue.
    bool             m_is_method;    // Internal state.
    sc_method_handle m_method_p;     // Method implementing this process.
    char*            m_name_p;       // Name of the process.
    int              m_references_n; // Count references to this object.
    sc_thread_handle m_thread_p;     // Thread implementing this process.

  protected:
    static sc_process_base* m_delete_q; // Queue of processes to delete.
};

//------------------------------------------------------------------------------
//"sc_process_base::operator sc_method_handle"
//
// This operator returns the hand of the method used to implement this
// process.
//------------------------------------------------------------------------------
inline sc_process_base::operator sc_method_handle()
{
    return m_method_p;
}


//------------------------------------------------------------------------------
//"sc_process_base::operator sc_thread_handle"
//
// This operator returns the hand of the thread used to implement this
// process.
//------------------------------------------------------------------------------
inline sc_process_base::operator sc_thread_handle()
{
    return m_thread_p;
}



//------------------------------------------------------------------------------
//"sc_process_base::reference_decrement"
//
// This inline method decrements the number of outstanding references to this 
// object instance. If the number of references goes to zero, this object
// instance is placed on the deletion queue.
//------------------------------------------------------------------------------
inline void sc_process_base::reference_decrement()
{
    m_references_n--;
    if ( m_references_n == 0 )
    {
        m_delete_p = m_delete_q;
        m_delete_q = this;
    }
}


//------------------------------------------------------------------------------
//"sc_process_base::reference_increment"
//
// This inline method increments the number of outstanding references to this 
// object instance.
//------------------------------------------------------------------------------
inline void sc_process_base::reference_increment()
{
    m_references_n++;
}


//=============================================================================
// CLASS sc_process_handle
//
// This class provides access to an sc_process_base object instance in a
// manner which allows some persistence after the deletion of the actual 
// process.
//=============================================================================
class sc_process_handle {
    friend class sc_join;
  public:
    inline explicit sc_process_handle( sc_process_base* target_p );
    inline sc_process_handle( const sc_process_handle& orig );
    inline ~sc_process_handle();
    inline sc_process_handle& operator = ( const sc_process_handle& src );
    inline void wait();

  protected:
    sc_process_base* m_target_p; // Target for this object instance.

  private:
    // disabled
    sc_process_handle();
};


//------------------------------------------------------------------------------
//"sc_process_handle::sc_process_handle - pointer constructor"
//
// This version of the object instance constructor for this class creates
// an object instance whose target is the supplied sc_process_base object
// instance.
//     target_p -> sc_process_base object instance this is handle for.
//------------------------------------------------------------------------------
inline sc_process_handle::sc_process_handle( sc_process_base* target_p )
{
    if ( !target_p )
    {
        SC_REPORT_ERROR(SC_ID_NULL_PROCESS_HANDLE, 0);
    }

    m_target_p = target_p;
    target_p->reference_increment();
}


//------------------------------------------------------------------------------
//"sc_process_handle::sc_process_handle - copy constructor"
//
// This version of the object instance constructor for this class provides
// the copy constructor for the class. It clones the supplied original
// handle and increments the references to its target.
//     orig = sc_process_handle object instance to be copied from.
//------------------------------------------------------------------------------
inline sc_process_handle::sc_process_handle( const sc_process_handle& orig )
{
    m_target_p = orig.m_target_p;
    m_target_p->reference_increment();
}

//------------------------------------------------------------------------------
//"sc_process_handle::operator = - copy constructor"
//
// This version of the object instance constructor for this class provides
// the copy constructor for the class. It clones the supplied original
// handle and increments the references to its target.
//     orig = sc_process_handle object instance to be copied from.
//------------------------------------------------------------------------------
inline sc_process_handle& 
sc_process_handle::operator = ( const sc_process_handle& orig )
{
    m_target_p->reference_decrement();
    m_target_p = orig.m_target_p;
    m_target_p->reference_increment();
    return *this;
}


//------------------------------------------------------------------------------
//"sc_process_handle::~sc_process_handle"
//
// This is the object instance destructor for this class. It decrements
// the reference count for its target.
//------------------------------------------------------------------------------
inline sc_process_handle::~sc_process_handle()
{
    if ( m_target_p ) m_target_p->reference_decrement();
}


//------------------------------------------------------------------------------
//"sc_process_handle::wait"
//
// This method suspends the current thread until the process represented by
// this object instance terminates execution.
//------------------------------------------------------------------------------
inline void sc_process_handle::wait()
{
    if ( m_target_p ) m_target_p->wait();
}


//=============================================================================
// CLASS sc_spawn_object<T>
//
// This templated helper class allows an object to provide the execution 
// semantics for a thread via its () operator. An instance of the supplied 
// execution object will be kept to provide the semantics when the thread is 
// scheduled for execution. The () operator does not return a value. An example 
// of an object that might be used for this helper function would be void BOOST 
// bound function or method. 
//
// This class is derived from sc_process_base and overloads 
// sc_process_base::semantics to provide the actual semantic content. 
//
//   sc_spawn_object(T object, const char* name, const sc_spawn_options* opt_p)
//     This is the object instance constructor for this class. It makes a
//     copy of the supplied object. The tp_call constructor is called
//     with an indication that this object instance should be reclaimed when
//     execution completes.
//         object   =  object whose () operator will be called to provide
//                     the thread semantics.
//         name_p   =  optional name for object instance, or zero.
//         opt_p    -> spawn options or zero.
//
//   virtual void semantics()
//     This virtual method provides the execution semantics for its thread.
//     It performs a () operation on m_object.
//=============================================================================
template<typename T>
class sc_spawn_object : public sc_process_base {
  public:
    sc_spawn_object( 
        T object, const char* name_p, const sc_spawn_options* opt_p=0 
    ) : 
        sc_process_base(name_p, opt_p), m_object(object)
    {
    }

    virtual void semantics()
    {
        m_object();
    }

  protected:
    T m_object;
};


//------------------------------------------------------------------------------
//"sc_spawn - semantic object with no return value"
//
// This inline function spawns a thread for execution. The execution semantics 
// for the thread being spawned will be provided by the supplied object 
// instance via its () operator. (E.g., a BOOST bound function) 
//     object   =   object instance providing the execution semantics via its 
//                  () operator.
//     name_p   =   optional name for object instance, or zero.
//     opt_p    ->  optional spawn options for thread, or zero for the default.
//------------------------------------------------------------------------------
template <typename T>
inline sc_process_handle sc_spawn( 
    T object, const char* name_p=0, const sc_spawn_options* opt_p=0 )
{ 
    sc_process_handle result(new sc_spawn_object<T>(object, name_p, opt_p));
    return result;
}

//=============================================================================
// CLASS sc_spawn_object_v<T> for all compilers except HP aCC
//              or
// CLASS sc_spawn_object_v<T, R> for HP aCC which tries to match this 
// one template argument class when the sc_spawn() declared above is
// invoked with 3 arguments or 2 arguments, and generates compiler errors.
//
// This templated helper class allows an object to provide the execution 
// semantics for a thread via its () operator. An instance of the supplied 
// object will be kept to provide the semantics when the thread is scheduled 
// for execution. The () operator returns a value, which will be stored at the 
// location specified by the supplied pointer. An example of an object that 
// might be used for this helper function would be valued BOOST bound function 
// or method. 
//
// This class is derived from sc_thread::tp_call and overloads tp_call::invoke 
// to provide the actual semantic content. 
//
//   sc_spawn_object_v( typename F::result_type* r_p, T f, const char* name_p,
//                      const sc_spawn_options* opt_p )
//       r_p      -> where to place the result of the function invocation.
//       f        =  information to be executed.
//       name_p   =  optional name for object instance, or zero.
//       opt_p    -> optional spawn options for thread, or zero for the default.
//     This is the object instance constructor for this class. It makes a
//     copy of the supplied object. The tp_call constructor is called
//     with an indication that this object instance should be reclaimed when
//     execution completes.
//         result_p -> where to place the value of the () operator.
//         object   =  object whose () operator will be called to provide
//                     the thread semantics.
//
//   virtual void semantics()
//     This virtual method provides the execution semantics for its thread.
//     It performs a () operation on m_object, placing the result at m_result_p.
//=============================================================================

//------------------------------------------------------------------------------
//"sc_spawn - semantic object with return value"
//
// This inline function spawns a thread for execution. The execution semantics 
// for the thread being spawned will be provided by the supplied object 
// instance via its () operator. (E.g., a BOOST bound function) That operator 
// returns a value, which will be placed in the supplied return location. 
//     object   =  object instance providing the execution semantics via its () 
//                 operator.
//     r_p      -> where to place the value of the () operator.
//     name_p   =  optional name for object instance, or zero.
//     opt_p    -> optional spawn options for thread, or zero for the default.
//------------------------------------------------------------------------------

#if !defined (__HP_aCC)

template<typename T>
class sc_spawn_object_v : public sc_process_base {
  public:
    sc_spawn_object_v( 
        typename T::result_type* r_p, T object, const char* name_p, 
        const sc_spawn_options* opt_p
    ) :
        sc_process_base( name_p, opt_p), m_object(object), m_result_p(r_p)
    {
    }

    virtual void semantics()
    {
        *m_result_p = m_object();
    }

  protected:
    T                        m_object;
    typename T::result_type* m_result_p;
};

template <typename T>
inline sc_process_handle sc_spawn( 
    typename T::result_type* r_p, 
    T object, 
    const char* name_p = 0,
    const sc_spawn_options* opt_p = 0)
{
    sc_process_handle result( 
        new sc_spawn_object_v<T>(r_p, object, name_p, opt_p)
    );
    return result;
}

#else
// for HP aCC

template<typename T, typename R>
class sc_spawn_object_v : public sc_process_base {
  public:
    sc_spawn_object_v( 
       R* r_p, T object, const char* name_p, 
        const sc_spawn_options* opt_p
    ) :
        sc_process_base( name_p, opt_p), m_object(object), m_result_p(r_p)
    {
    }

    virtual void semantics()
    {
        *m_result_p = m_object();
    }

  protected:
    T                        m_object;
    R*                       m_result_p;
};

template <typename T, typename R>
inline sc_process_handle sc_spawn( 
    R* r_p, 
    T object, 
    const char* name_p = 0,
    const sc_spawn_options* opt_p = 0)
{
    sc_process_handle result( 
        new sc_spawn_object_v<T, R>(r_p, object, name_p, opt_p)
    );
    return result;
}

#endif

} // namespace sc_core

#endif // SC_PROCESS_BASE_H
