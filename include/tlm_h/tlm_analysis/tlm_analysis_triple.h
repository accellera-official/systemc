
#ifndef __TLM_ANALYSIS_TRIPLE_H__
#define __TLM_ANALYSIS_TRIPLE_H__

//#include <systemc>

namespace tlm {

template< typename T>
struct analysis_triple {

  sc_core::sc_time start_time;
  T transaction;
  sc_core::sc_time end_time;

  analysis_triple() {}

  analysis_triple( const analysis_triple &triple ) {
    start_time = triple.start_time;
    transaction = triple.transaction;
    end_time = triple.end_time;
  }

  analysis_triple( const T &t ) {
    transaction = t;
  }

  operator T() { return transaction; }
  operator const T &() const { return transaction; }

};

} // namespace tlm

#endif
