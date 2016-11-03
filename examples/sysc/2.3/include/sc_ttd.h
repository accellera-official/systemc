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
#if !defined(sc_ttd_h_included)
#define sc_ttd_h_included

// This header file contains classes implementing a channel, in port, and 
// out port that may be used to transfer data via a "toggle-toggle" protocol.
// The toggle-toggle protocol uses two bits to specify the state of the channel,
// a read toggle, and a write toggle. The protocol has the following 
// characteristics:
//    (1) If the toggle values are not equal then there is data available in
//        the channel.
//    (2) When reading or writing the channel it is not necessary to wait
//        for an acknowledgment from the other side of the channel, one need 
//        only toggle the appropriate toggle value. At the next clock edge the 
//        change in toggle will be visible to the other side of the channel.
//    (3) A read can be accomplished in a single clock if data is available
//        (the toggle values differ.) 
//    (4) A write can be accomplished in a single clock if the channel is
//        empty (the toggle values are the same.)
//    (5) A call to the reset() method of the read and write ports is 
//        necessary to place the channel in its initial state.
//    (6) There is no "burst" mode when transferring data. Back to back writes
//        will always have a one clock "bubble" since the handshake must be
//        toggled. This differs from a ready-valid handshake, where leaving
//        ready and valid asserted allow the transfer of data on every clock.

// +============================================================================
// | sc_ttd_export_in<D> - EXPORT OF INPUT INTERFACE FOR A COMMUNICATION 
// |                       CHANNEL WITH A TOGGLE-TOGGLE HANDSHAKE
// | 
// | This class provides the interface necessary for binding an input for a 
// | data transfer channel that is controlled using a toggle-toggle handshake. 
// |
// +============================================================================
template<typename D>
class sc_ttd_export_in
{
  public:
    sc_ttd_export_in(const char* name=sc_gen_unique_name("sc_ttd_export_in")) :
        m_data( (std::string(name)+"m_data").c_str() ),
        m_read_toggle( (std::string(name)+"m_read_toggle").c_str() ),
        m_write_toggle( (std::string(name)+"m_write_toggle").c_str() )
    {}

  public:
    sc_export<sc_signal_in_if<D> >   m_data;           // data to transfer.
    sc_export<sc_signal_out_if<bool> > m_read_toggle;  // toggle for read side.
    sc_export<sc_signal_in_if<bool> >  m_write_toggle; // toggle for write side.
};

// +============================================================================
// | sc_ttd_export_out<D> - EXPORT OF OUTPUT SIGNALS FOR A COMMUNICATION 
// |                        CHANNEL WITH A TOGGLE-TOGGLE HANDSHAKE
// | 
// | This class provides the interface necessary for binding an output for a
// | data transfer channel that is controlled using a toggle-toggle handshake. 
// |
// +============================================================================
template<typename D>
class sc_ttd_export_out
{
  public:
    sc_ttd_export_out(const char* name=sc_gen_unique_name("sc_ttd_export_out")):
        m_data( (std::string(name)+"m_data").c_str() ),
        m_read_toggle( (std::string(name)+"m_read_toggle").c_str() ),
        m_write_toggle( (std::string(name)+"m_write_toggle").c_str() )
    {}

  public:
    sc_export<sc_signal_out_if<D> >     m_data;         // data to transfer.
    sc_export<sc_signal_in_if<bool> >   m_read_toggle;  // toggle for read side.
    sc_export<sc_signal_out_if<bool> >  m_write_toggle; // toggle for write side.
};

// +============================================================================
// | sc_ttd_in<D> - PORT TO READ VALUES FROM AN sc_ttd CHANNEL
// | 
// | This class provides the interface to read signals from an sc_ttd channel.
// +============================================================================
template<typename D>
class sc_ttd_in
{
  public: // constructor:
    sc_ttd_in( const char* name=sc_gen_unique_name("sc_ttd_in") ) :
        m_data( (std::string(name)+"_data").c_str() ), 
        m_read_toggle( (std::string(name)+"_ready").c_str() ), 
        m_write_toggle( (std::string(name)+"_valid").c_str() ) 
    { }

  public: // channel binding method and operator:
    template<typename CHANNEL>
    inline void bind( CHANNEL& channel )
    {
        m_data(channel.m_data);
        m_read_toggle(channel.m_read_toggle);
        m_write_toggle(channel.m_write_toggle);
    }

    template<typename CHANNEL>
    inline void operator () ( CHANNEL& channel )
    {
        bind(channel);
    }

  public: // channel access methods and operators:
    inline bool nb_can_read()
    {
        return m_write_toggle.read() != m_read_toggle.read();
    }

    inline bool nb_read( D& data )
    {
	data = m_data;
	if ( m_write_toggle.read() != m_read_toggle.read() )
	{
	    m_read_toggle = !m_read_toggle;
	    return true;
	}
	else
	{
	    return false;
	}
    }

    inline void reset()
    {
        m_read_toggle = false;
    }

    inline D read()
    {
	do { ::wait(); } while (m_write_toggle.read() == m_read_toggle.read());
	m_read_toggle = !m_read_toggle;
	return m_data.read();
    }

    inline operator D ()
    {
	return read();
    }

  protected: // channel connections:
    sc_in<D>     m_data;         // data to be transferred.
    sc_out<bool> m_read_toggle;  // toggle for the read side of the channel.
    sc_in<bool>  m_write_toggle; // toggle for the write side of the channel.
};

// +============================================================================
// | sc_ttd_out<D> - PORT TO WRITE VALUES TO AN sc_ttd CHANNEL
// | 
// | This class provides the interface to write values to an sc_ttd channel.
// +============================================================================
template<typename D>
class sc_ttd_out
{
  public: // constructor:
    sc_ttd_out( const char* name=sc_gen_unique_name("sc_ttd_out") ) :
        m_data( (std::string(name)+"_data").c_str() ), 
        m_read_toggle( (std::string(name)+"_ready").c_str() ), 
        m_write_toggle( (std::string(name)+"_valid").c_str() ) 
    { }

  public: // channel binding method and operator:
    template<typename CHANNEL>
    inline void bind( CHANNEL& channel )
    {
        m_data(channel.m_data);
        m_read_toggle(channel.m_read_toggle);
        m_write_toggle(channel.m_write_toggle);
    }
    template<typename CHANNEL>
    inline void operator () ( CHANNEL& channel )
    {
        bind(channel);
    }

  public: // channel access methods and operators:
    inline bool nb_can_write()
    {
        return m_write_toggle.read() == m_read_toggle.read();
    }

    inline bool nb_write( const D& data )
    {
	if ( m_write_toggle.read() == m_read_toggle.read() )
	{
	    m_data = data;
	    m_write_toggle = !m_write_toggle;
	    return true;
	}
	else
	{
	    return false;
	}
    }

    inline void reset()
    {
        m_write_toggle = false;
    }

    inline void write( const D& data )
    {
	do { ::wait(); } while (m_write_toggle.read() != m_read_toggle.read());
	m_data = data;
        m_write_toggle = !m_write_toggle;
    }

    inline const D& operator = ( const D& data )
    {
        write( data );
	return data;
    }

  protected: // channel connections:
    sc_out<D>    m_data;          // data to be transferred.
    sc_in<bool>  m_read_toggle;  // toggle for the read side of the channel.
    sc_out<bool> m_write_toggle; // toggle for write side of the channel.
};

// +============================================================================
// | sc_ttd<D> - COMMUNICATION CHANNEL WITH A TOGGLE-TOGGLE HANDSHAKE
// | 
// | This class provides the signals necessary for a data transfer channel 
// | that is controlled using a toggle-toggle handshake. 
// |
// +============================================================================
template<typename D>
class sc_ttd
{
  public:
    typedef sc_ttd<D>     channel; // channel.
    typedef sc_ttd_in<D>  in;      // input port.
    typedef sc_ttd_out<D> out;     // output port.
    
  public:
    sc_signal<D>    m_data;         // data to be transferred.
    sc_signal<bool> m_read_toggle;  // toggle for read side of the channel.
    sc_signal<bool> m_write_toggle; // toggle for write side of the channel.
};

#endif // !defined(sc_ttd_h_included)
