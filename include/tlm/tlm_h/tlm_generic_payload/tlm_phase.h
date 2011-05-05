/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#ifndef __TLM_PHASE_H__
#define __TLM_PHASE_H__

#include <string>
#include <iostream>
#include <vector>

namespace tlm {

//enum tlm_phase { BEGIN_REQ, END_REQ, BEGIN_RESP, END_RESP };
  
enum tlm_phase_enum { UNINITIALIZED_PHASE=0, BEGIN_REQ=1, END_REQ, BEGIN_RESP, END_RESP };

inline unsigned int create_phase_number(){
  static unsigned int number=END_RESP+1;
  return number++;
}

inline std::vector<const char*>& get_phase_name_vec(){
  static std::vector<const char*> phase_name_vec(END_RESP+1, (const char*)NULL);
  return phase_name_vec;
}

class tlm_phase{
public:
  tlm_phase(): m_id(0) {}
  tlm_phase(unsigned int id): m_id(id){}
  tlm_phase(const tlm_phase_enum& standard): m_id((unsigned int) standard){}
  tlm_phase& operator=(const tlm_phase_enum& standard){m_id=(unsigned int)standard; return *this;}
  operator unsigned int() const{return m_id;}
  
private:
  unsigned int m_id;  
};

inline
std::ostream& operator<<(std::ostream& s, const tlm_phase& p){
  switch ((unsigned int)p){
    case UNINITIALIZED_PHASE: s<<"UNINITIALIZED_PHASE"; break;
    case BEGIN_REQ:  s<<"BEGIN_REQ"; break;
    case END_REQ:    s<<"END_REQ"; break;
    case BEGIN_RESP: s<<"BEGIN_RESP"; break;
    case END_RESP:   s<<"END_RESP"; break;
    default:
      s<<get_phase_name_vec()[(unsigned int)p]; return s;      
  }
  return s;
}
  
#define TLM_DECLARE_EXTENDED_PHASE(name_arg) \
class tlm_phase_##name_arg:public tlm::tlm_phase{ \
public:\
static const tlm_phase_##name_arg& get_phase(){static tlm_phase_##name_arg tmp; return tmp;}\
private:\
tlm_phase_##name_arg():tlm::tlm_phase(tlm::create_phase_number()){tlm::get_phase_name_vec().push_back(get_char_##name_arg());};\
tlm_phase_##name_arg(const tlm_phase_##name_arg&); \
tlm_phase_##name_arg& operator=(const tlm_phase_##name_arg&); \
static inline const char* get_char_##name_arg(){static const char* tmp=#name_arg; return tmp;} \
}; \
static const tlm_phase_##name_arg& name_arg=tlm_phase_##name_arg::get_phase()

// for backwards-compatibility
#define DECLARE_EXTENDED_PHASE(NameArg) \
    TLM_DECLARE_EXTENDED_PHASE( NameArg )

} // namespace tlm

#endif /* TLM_PHASE_HEADER */
