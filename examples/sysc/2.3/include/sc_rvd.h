/****************************************************************************
*
* The following code is derived, directly or indirectly, from the SystemC
* source code Copyright (c) 1996-2010 by all Contributors.
* All Rights reserved.
*
* The contents of this file are subject to the restrictions and limitations
* set forth in the SystemC Open Source License Version 2.4 (the "License");
* You may not use this file except in compliance with such restrictions and
* limitations. You may obtain instructions on how to receive a copy of the
* License at http://www.systemc.org/. Software distributed by Contributors
* under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
* ANY KIND, either express or implied. See the License for the specific
* language governing rights and limitations under the License.
*
****************************************************************************/
#if !defined(sc_rvd_h_included)
#define sc_rvd_h_included


// This header file contains classes implementing a channel, in port, and 
// out port that may be used to transfer data via a "ready-valid" protocol.
// The ready-valid protocol uses two bits to specify the state of the channel,
// a ready assertion for the read side of the channel, and a valid assertion
// for the write side of the channel. The protocol has the following 
// characteristics:
//    (1) If the valid signal is true there is data available in the channel,
//        if the ready signal is true the read side of the channel is ready
//        to read data.
//    (2) Data transfers on clocks where the ready and valid signals are
//        both asserted.
//    (3) When writing data to the channel it is necessary to wait for the
//        ready signal to be asserted before the valid signal can be deasserted.
//    (4) When reading data from the channel it is necessary to wait for
//        valid signal to be asserted before data can be transferred, and 
//        the ready signal deasserted.
//    (5) A call to the reset() method of the read and write ports is 
//        necessary to place the channel in its initial state.
//    (6) There is always a one clock "bubble" on the first data transfer
//        in a sequence. However, multiple values can be transferred, one
//        value per clock after that by having the read and write sides
//        leave their handshake signals asserted. When the transfer is 
//        complete the signals should be deasserted.

// +============================================================================
// | sc_rvd_export_in<D> - EXPORT OF INPUT INTERFACE FOR A COMMUNICATION 
// |                       CHANNEL WITH A TOGGLE-TOGGLE HANDSHAKE
// | 
// | This class provides the interface necessary for binding an input for a
// | data transfer channel that is controlled using a ready-valid handshake. 
// |
// +============================================================================
template<typename D>
class sc_rvd_export_in
{
  public:
    sc_rvd_export_in(const char* name=sc_gen_unique_name("sc_rvd_export_in")) :
        m_data( (std::string(name)+"m_data").c_str() ),
        m_ready( (std::string(name)+"m_ready").c_str() ),
        m_valid( (std::string(name)+"m_valid").c_str() )
    {}

  public:
    sc_export<sc_signal_in_if<D> >     m_data;   // data to transfer.
    sc_export<sc_signal_out_if<bool> > m_ready;  // valid signal.
    sc_export<sc_signal_in_if<bool> >  m_valid;  // ready signal.
};

// +============================================================================
// | sc_rvd_export_out<D> - EXPORT OF INTERFACE SIGNALS FOR A COMMUNICATION 
// |                        CHANNEL WITH A TOGGLE-TOGGLE HANDSHAKE
// | 
// | This class provides the interface necessary for binding an output for a
// | data transfer channel that is controlled using a ready-valid handshake. 
// |
// +============================================================================
template<typename D>
class sc_rvd_export_out
{
  public:
    sc_rvd_export_out(const char* name=sc_gen_unique_name("sc_rvd_export_out")):
        m_data( (std::string(name)+"m_data").c_str() ),
        m_ready( (std::string(name)+"m_ready").c_str() ),
        m_valid( (std::string(name)+"m_valid").c_str() )
    {}

  public:
    sc_export<sc_signal_out_if<D> >     m_data;   // data to transfer.
    sc_export<sc_signal_in_if<bool> >   m_ready;  // valid signal.
    sc_export<sc_signal_out_if<bool> >  m_valid;  // ready signal.
};

// +============================================================================
// | sc_rvd_in<D> - PORT TO READ VALUES FROM AN sc_rvd CHANNEL
// | 
// | This class provides the interface to read values from an sc_rvd channel.
// +============================================================================
template<typename D>
class sc_rvd_in
{
  public: // constructor:
    sc_rvd_in( const char* name=sc_gen_unique_name("sc_rvd_in") ) :
        m_data( (std::string(name)+"_data").c_str() ), 
        m_ready( (std::string(name)+"_ready").c_str() ), 
        m_valid( (std::string(name)+"_valid").c_str() ) 
    { }

  public: // channel binding method and operator:
    template<typename CHANNEL>
    inline void bind( CHANNEL& channel )
    {
        m_data(channel.m_data);
        m_ready(channel.m_ready);
        m_valid(channel.m_valid);
    }

    template<typename CHANNEL>
    inline void operator () ( CHANNEL& channel )
    {
        bind(channel);
    }

  public: // channel access methods and operators:
    inline bool nb_can_read()
    {
        return m_valid.read();
    }

    inline bool nb_read( D& data )
    {
	data = m_data;
	if ( m_valid.read() == true )
	{
	    m_ready = true;
	    return true;
	}
	else
	{
	    return false;
	}
    }

    inline void reset()
    {
        m_ready = false;
    }

    inline D read()
    {
        m_ready = true;
	do { ::wait(); } while ( m_valid.read() == false );
	m_ready = false;
	return m_data.read();
    }

    inline operator D ()
    {
	return read();
    }

  protected: // channel connections:
    sc_in<D>     m_data;  // data to be transferred.
    sc_out<bool> m_ready; // true if this port is ready to read a value.
    sc_in<bool>  m_valid; // true if m_data has a value to be read.
};

// +============================================================================
// | sc_rvd_out<D> - PORT TO WRITE VALUES TO AN sc_rvd CHANNEL
// | 
// | This class provides the interface to write values to an sc_rvd channel.
// +============================================================================
template<typename D>
class sc_rvd_out
{
  public: // constructor:
    sc_rvd_out( const char* name=sc_gen_unique_name("sc_rvd_out") ) :
        m_data( (std::string(name)+"_data").c_str() ), 
        m_ready( (std::string(name)+"_ready").c_str() ), 
        m_valid( (std::string(name)+"_valid").c_str() ) 
    { }

  public: // channel binding method and operator:
    template<typename CHANNEL>
    inline void bind( CHANNEL& channel )
    {
        m_data(channel.m_data);
        m_ready(channel.m_ready);
        m_valid(channel.m_valid);
    }
    template<typename CHANNEL>
    inline void operator () ( CHANNEL& channel )
    {
        bind(channel);
    }

  public: // channel access methods and operators:
    inline bool nb_can_write()
    {
        return m_valid.read() == false || m_ready.read() == true;
    }

    inline bool nb_write( const D& data )
    {
	if ( m_ready.read() == true )
	{
	    m_data = data;
	    m_valid = true;
	    return true;
	}
	else
	{
	    return false;
	}
    }

    inline void reset()
    {
        m_valid = false;
    }

    inline void write( const D& data )
    {
	m_data = data;
        m_valid = true;
	do { ::wait(); } while ( m_ready.read() == false );
	m_valid = false;
    }

    inline const D& operator = ( const D& data )
    {
        write( data );
	return data;
    }

  protected: // channel connections:
    sc_out<D>    m_data;  // data to be transferred.
    sc_in<bool>  m_ready; // true if this port is ready to read a value.
    sc_out<bool> m_valid; // true if m_data has a value to be read.
};

// +============================================================================
// | sc_rvd<D> - COMMUNICATION CHANNEL WITH A READY-VALID HANDSHAKE
// | 
// | This class provides the signals necessary for a data transfer channel 
// | that is controlled using a ready-valid handshake. 
// |
// +============================================================================
template<typename D>
class sc_rvd
{
  public:
    typedef sc_rvd<D>     channel; // channel.
    typedef sc_rvd_in<D>  in;      // input port.
    typedef sc_rvd_out<D> out;     // output port.
    
  public:
    sc_signal<D>    m_data;  // data to be transferred.
    sc_signal<bool> m_ready; // true if the channel reader if ready for data.
    sc_signal<bool> m_valid; // true if the channel writer has provided data.
};

#endif // !defined(sc_rvd_h_included)
