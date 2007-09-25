

/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
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


#ifndef TLM_ANNOTATED_MASTER_SLAVE_IFS_HEADER
#define TLM_ANNOTATED_MASTER_SLAVE_IFS_HEADER

// 
// req/rsp combined interfaces
//

#include "tlm_annotated/tlm_annotated_ifs/tlm_annotated_ifs.h"

using tlm_core::tlm_nonblocking_master_if;
using tlm_core::tlm_nonblocking_slave_if;
using tlm_core::tlm_master_if;
using tlm_core::tlm_slave_if;

// nonblocking

template < typename REQ , typename RSP >
class tlm_annotated_nonblocking_master_if : 
  public virtual tlm_annotated_nonblocking_put_if< REQ > ,
  public virtual tlm_annotated_nonblocking_get_peek_if< RSP > ,
  public virtual tlm_nonblocking_master_if < REQ , RSP >{};

template < typename REQ , typename RSP >
class tlm_annotated_nonblocking_slave_if: 
  public virtual tlm_annotated_nonblocking_put_if< RSP  > ,
  public virtual tlm_annotated_nonblocking_get_peek_if< REQ > ,
  public virtual tlm_nonblocking_slave_if< REQ , RSP > {};

// combined

template < typename REQ , typename RSP >
class tlm_annotated_master_if : 
  public virtual tlm_annotated_put_if< REQ  > ,
  public virtual tlm_annotated_get_peek_if< RSP > ,
  public virtual tlm_annotated_nonblocking_master_if< REQ , RSP > ,
  public virtual tlm_master_if< REQ , RSP > {};

template < typename REQ , typename RSP>
class tlm_annotated_slave_if : 
  public virtual tlm_annotated_put_if< RSP > ,
  public virtual tlm_annotated_get_peek_if< REQ > ,
  public virtual tlm_annotated_nonblocking_slave_if< REQ , RSP > ,
  public virtual tlm_slave_if< REQ , RSP >{};

#endif
