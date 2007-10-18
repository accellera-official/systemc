/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#ifndef SIMPLE_SOCKET_UTILS_H
#define SIMPLE_SOCKET_UTILS_H

//#include "tlm.h"

namespace simple_socket_utils {
  
  class simple_socket_user {
  public:
    static simple_socket_user& instance()
    {
      static simple_socket_user* theInstance = 0;

      if (!theInstance) {
        theInstance = new simple_socket_user();
      }
      return *theInstance;
    }

    int get_user_id() const { return mUserId; }
    void set_user_id(int id) { mUserId = id; }

  private:
    int mUserId;
  };

}

#define REGISTER_SOCKETPROCESS(socket, process) \
  socket.CB(this, &SC_CURRENT_USER_MODULE::process, 0)

#define REGISTER_SOCKETPROCESS_USER(socket, process, id) \
  socket.CB(this, &SC_CURRENT_USER_MODULE::process, id)

#endif
