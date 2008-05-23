
/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

#ifndef BASIC_PROTOCOL_HEADER
#define BASIC_PROTOCOL_HEADER

#include <systemc.h>

//
// basic protocol definition
//

#undef ERROR // for those compilers naughty enough to define it !

namespace basic_protocol
{

enum basic_request_type
{
  READ , WRITE
};

enum basic_status
{

  ERROR , SUCCESS

};

template< typename ADDRESS , typename DATA >
class basic_request
{
public:
  basic_request_type type;

  ADDRESS &get_address() { return a; } // makes it routable

  ADDRESS a;
  DATA d;

};

template< typename DATA >
class basic_response
{
public:
  basic_response() : status( ERROR ) {} // makes it routable

  basic_request_type type;
  basic_status status;

  DATA d;

};

template< typename ADDRESS , typename DATA >
class basic_if
{
public:
  virtual basic_status write( const ADDRESS &a , const DATA &d ) = 0;
  virtual basic_status read( const ADDRESS &a , DATA &d ) = 0;

  virtual ~basic_if(void) {}

};

} // end namespace



template< typename ADDRESS , typename DATA >
ostream &operator<<( ostream &fsout ,
                     const basic_protocol::basic_request<ADDRESS,DATA> &req ) {

  switch( req.type )
  {
  case basic_protocol::READ :
    fsout << "Read Request : address = " << req.a << endl;
    break;
  case basic_protocol::WRITE :
    fsout << "Write Request : address,data =  " << req.a << " , " << req.d << endl;
    break;
  default :
    fsout << "Error : no request type" << endl;
    break;
  }

  return fsout;
}

template< typename DATA >
ostream &operator<<( ostream &fsout ,
                     const basic_protocol::basic_response<DATA> &req ) {

  if( req.status == basic_protocol::ERROR ) {
    fsout << "Error Response" << endl;
    return fsout;
  }
  
  switch( req.type )
  {
  case basic_protocol::READ :
    fsout << "Successful Read Response : data = " << req.d << endl;
    break;
  case basic_protocol::WRITE :
    fsout << "Successful Write Response" << endl;
    break;
  default :
    fsout << "Error : no request type ( in response )" << endl;
    break;
  }

  return fsout;

}

template< typename ADDRESS , typename DATA >
bool operator==( const basic_protocol::basic_request<ADDRESS,DATA> &req1 ,
                 const basic_protocol::basic_request<ADDRESS,DATA> &req2 )
{
  if( req1.type != req2.type ) {
    return false;
  }  
  
  if( req1.a != req2.a ) {
    return false;
  } 
 
  if( req1.type == basic_protocol::WRITE && req1.d != req2.d ) {
     return false;
  }
  
  return true;

}

template< typename ADDRESS , typename DATA >
bool operator!=( const basic_protocol::basic_request<ADDRESS,DATA> &req1 ,
                 const basic_protocol::basic_request<ADDRESS,DATA> &req2 ) {
                    
  return !(req1 == req2);                    
}


#endif
