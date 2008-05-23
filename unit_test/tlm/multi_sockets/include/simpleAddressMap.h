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

#ifndef __simpleAddressMap_h__
#define __simpleAddressMap_h__

#include <systemc>
#include <map>


//--------------------------------------------------------------------------
/**
 * Simple address map implementation for the generic protocol.
 */
//--------------------------------------------------------------------------
class SimpleAddressMap
{
  typedef std::map<sc_dt::uint64, unsigned int> mapType;
  typedef std::map<sc_dt::uint64, unsigned int>::iterator addressMapIterator;

public:
  SimpleAddressMap() {}

  //--------------------------------------------------------------------------
  /**
   * Check for overlapping address ranges
   */
  //--------------------------------------------------------------------------
  void checkSanity()
  {
    addressMapIterator pos;
    for (pos=m_addressMap.begin();pos!=m_addressMap.end();++pos){
      if(pos->second!=255)
        SC_REPORT_ERROR("SimpleAddressMap","Overlapping Address Regions.");
      else
        ++pos;
      if(pos->second==255)
        SC_REPORT_ERROR("SimpleAddressMap","Overlapping Address Regions.");
    }
    std::cout<<"Address check successful."<<std::endl;
  }


  //--------------------------------------------------------------------------
  /**
   * Print map
   */
  //--------------------------------------------------------------------------
  void dumpMap()
  {
    std::cout<<"SimpleAddressMap: printing the sorted MAP:"<<std::endl;
    addressMapIterator pos;
    for (pos=m_addressMap.begin();pos!=m_addressMap.end();++pos){
      if(pos->second==255)
        printf("key: %x    value: %i \n", (unsigned int) ((pos->first+1)>>1)-1, pos->second);
      else
        printf("key: %x    value: %i \n", (unsigned int) (pos->first>>1)-1, pos->second);
    }
  }


  //--------------------------------------------------------------------------
  /**
   * Decode slave address.
   * @param address_ A slave address.
   * @return The decoded slave port number. On error, the value 255 is returned.
   */
  //--------------------------------------------------------------------------
  unsigned int decode(sc_dt::uint64 address_)
  {
    addressMapIterator lbound;

    lbound=m_addressMap.lower_bound((address_+1)<<1);
    if((lbound->second == 255) | (lbound==m_addressMap.end())){
      SC_REPORT_ERROR("SimpleAddressMap", "Address does not match any registered address range.");
    }
    else{
      return lbound->second;
    }
    return 255;
  }

  const sc_dt::uint64& get_max(){
    if (m_addressMap.size()){
      addressMapIterator i=(m_addressMap.end());
      i--;
      retval=(i->first>>1)-1;
      return retval;
    }
    else {
      SC_REPORT_ERROR("SimpleAddressMap", "get_max() called on empty address map.");
      return retval;
    }
  }

  const sc_dt::uint64& get_min(){
    if (m_addressMap.size()){
      addressMapIterator i=(m_addressMap.begin());
      retval=((i->first+1)>>1)-1;
      return retval;
    }
    else {
      SC_REPORT_ERROR("SimpleAddressMap", "get_min() called on empty address map.");
      return retval;
    }
  }

  //--------------------------------------------------------------------------
  /**
   * Insert a slave into the address map
   */
  //--------------------------------------------------------------------------
  void insert(sc_dt::uint64 baseAddress_, sc_dt::uint64 highAddress_, unsigned int portNumber_)
  {
    if(baseAddress_>highAddress_)
      SC_REPORT_ERROR("Address Map", "Base address must be lower than high address.");
    if(portNumber_>=255)
      SC_REPORT_ERROR("Address Map", "Only ;-) 255 targets can be handled.");
    m_addressMap.insert(mapType::value_type(((baseAddress_+1)<<1)-1,    255    ));
    m_addressMap.insert(mapType::value_type( (highAddress_+1)<<1   ,portNumber_));
  }

private:
  sc_dt::uint64 retval;
  /// the address map
  mapType m_addressMap;
};




#endif
