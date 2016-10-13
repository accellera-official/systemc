/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_spawn_options.cpp -- Process spawning options implementation.

  Original Authors: Andy Goodrich, Forte Design Systems, 17 June 2003
                    Stuart Swan, Cadence,
                    Bishnupriya Bhattacharya, Cadence Design Systems,
                    25 August, 2003

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#include "sysc/kernel/sc_spawn_options.h"
#include "sysc/kernel/sc_reset.h"

namespace sc_core {

// +======================================================================
// | CLASS sc_spawn_reset_base - Class to do a generic access to an 
// |                             sc_spawn_rest object instance
// +======================================================================
class sc_spawn_reset_base
{
  public:
    sc_spawn_reset_base( bool async, bool level )
      : m_async( async ), m_level(level)
    {}
    virtual ~sc_spawn_reset_base() {}
    virtual void specify_reset() = 0;

  protected:
    bool m_async;   // = true if async reset.
    bool m_level;   // level indicating reset.
};

// +======================================================================
// | CLASS sc_spawn_reset<SOURCE>
// |  - Reset specification for sc_spawn_options.
// +======================================================================
template<typename SOURCE>
class sc_spawn_reset : public sc_spawn_reset_base
{
  public:
    sc_spawn_reset( bool async, const SOURCE& source, bool level )
      : sc_spawn_reset_base(async, level), m_source(source)
    {}
    virtual ~sc_spawn_reset() {}
    virtual void specify_reset()
    {
        sc_reset::reset_signal_is( m_async, m_source, m_level );
    }

  protected:
    const SOURCE& m_source; // source of reset signal.
};

// +======================================================================
// | CLASS sc_spawn_options (implementation)
// |
// +======================================================================

sc_spawn_options::~sc_spawn_options()
{
    std::vector<sc_spawn_reset_base*>::size_type resets_n = m_resets.size();
    for ( std::vector<sc_spawn_reset_base*>::size_type reset_i = 0; reset_i < resets_n; reset_i++ )
        delete m_resets[reset_i];
}

#define SC_DEFINE_RESET_SIGNALS( Port )                       \
  /* asynchronous reset */                                    \
  void                                                        \
  sc_spawn_options::                                          \
    async_reset_signal_is ( const Port & port, bool level )   \
    {                                                         \
        m_resets.push_back(                                   \
            new sc_spawn_reset< Port >(true, port, level) );  \
    }                                                         \
  /* sync reset */                                            \
  void                                                        \
  sc_spawn_options::                                          \
    reset_signal_is ( const Port & port, bool level )         \
    {                                                         \
        m_resets.push_back(                                   \
            new sc_spawn_reset< Port >(false, port, level) ); \
    }

SC_DEFINE_RESET_SIGNALS( sc_in<bool> )
SC_DEFINE_RESET_SIGNALS( sc_inout<bool> )
SC_DEFINE_RESET_SIGNALS( sc_out<bool> )
SC_DEFINE_RESET_SIGNALS( sc_signal_in_if<bool> )

#undef SC_DEFINE_RESET_SIGNALS

void
sc_spawn_options::specify_resets() const
{
    std::vector<sc_spawn_reset_base*>::size_type resets_n; // number of reset specifications to process.
    resets_n = m_resets.size();
    for ( std::vector<sc_spawn_reset_base*>::size_type reset_i = 0; reset_i < resets_n; reset_i++ )
        m_resets[reset_i]->specify_reset();
}

} // namespace sc_core

// Taf!
