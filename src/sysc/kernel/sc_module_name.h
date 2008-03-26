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

  sc_module_name.h -- An object used to help manage object names 
                      and hierarchy.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_MODULE_NAME_H
#define SC_MODULE_NAME_H


namespace sc_core {

class sc_simcontext;
class sc_module;


// ----------------------------------------------------------------------------
//  CLASS : sc_module_name
//
//  Module name class.
// ----------------------------------------------------------------------------

class sc_module_name
{
    friend class sc_module;
    friend class sc_object_manager;

public:

    sc_module_name( const char* );
    sc_module_name( const sc_module_name& );

    ~sc_module_name();

    operator const char*() const;

private:

    const char*     m_name;
    sc_module*      m_module;
    sc_module_name* m_next;
    sc_simcontext*  m_simc;
    bool            m_pushed;

private:

    // disabled
    sc_module_name();
    sc_module_name& operator = ( const sc_module_name& );
};

} // namespace sc_core

#endif
