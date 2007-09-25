
#ifndef ANALYSIS_IF_HEADER
#define ANALYSIS_IF_HEADER

//#include <systemc.h>

template < typename T >
class analysis_if : public virtual sc_interface {
 public:
  virtual void write( const T &t ) = 0;
};

template < typename T >
class delayed_analysis_if : public virtual sc_interface {
 public:

  virtual void write( const T &transaction ,
		      const sc_time &time ) = 0;
};

#endif
