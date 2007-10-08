
#ifndef ANALYSIS_TRIPLE
#define ANALYSIS_TRIPLE

//#include "systemc.h"

template< typename T>
struct analysis_triple {

  sc_time start_time;
  T transaction;
  sc_time end_time;

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


#endif
