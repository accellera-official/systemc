
#ifndef __TLM_ANALYSIS_IF_H__
#define __TLM_ANALYSIS_IF_H__

//#include <systemc>

template < typename T >
class analysis_if : public virtual sc_core::sc_interface {
 public:
  virtual void write( const T &t ) = 0;
};

template < typename T >
class delayed_analysis_if : public virtual sc_core::sc_interface {
 public:

  virtual void write( const T &transaction ,
		      const sc_core::sc_time &time ) = 0;
};

#endif
