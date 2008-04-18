/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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


#ifndef __TLM_ENDIAN_CONV_H__
#define __TLM_ENDIAN_CONV_H__

#include <systemc>
#include "tlm_gp.h"


namespace tlm {


/*
Tranaction-Level Modelling
Endianness Helper Functions

TO DO
+ Does not yet support data size greater than bus width.
+ Change generic loop to cut off any un-enabled header on a transaction
+ Add a further function set with support for byte-enable length
+ Work in-place for aligned RD transactions without byte enables

DESCRIPTION
A set of functions for helping users to get the endianness
right in their TLM models of system initiators.  These functions are
primarily for use within an initiator.  They can not be used as-is outside
an initiator because the extension used to store context will not work
if cascaded.  However a copy of this code with a differently-named
extension type may be useable in bridges, etc..

These functions are not compulsory.  There are other legitimate ways to
achieve the same functionality.  If extra information is available at
compile time about the nature of an initiator's transactions, this can
be exploited to accelerate simulations by creating further functions
similar to those in this file.  In general a functional transaction can be
described in more than one way by a TLM-2 GP object.

The functions invert the endianness of a GP object, either on request or
response.  They should only be used when the initiator's endianness
does not match the host's endianness.  They assume 'arithmetic mode'
meaning that within a data word the byte order is always host-endian.
For non-arithmetic mode initiators they can be used with a data word
size of 1 byte.

All the functions are templates, for example:

template<class DATAWORD> inline void
  to_hostendian_generic(tlm_generic_payload *txn, int sizeof_databus)

The template parameter provides the data word width.  Having this as a class
makes it easy to use it for copy and swap operations within the functions.
If the assignment operator for this class is overloaded, the endianness
conversion function may not have the desired effect.

All the functions have the same signature except for different names.

The principle is that a function to_hostendian_convtype() is called when the
initiator-endian transaction is created, and the matching function
from_hostendian_convtype() is called when the transaction is completed, for
example before read data can be used.  In some cases the from_ function is
redundant but an empty function is provided anyway.  It is strongly
recommended that the from_ function is called, in case it ceases to be
redundant in future versions of this code.

No context needs to be managed outside the two functions, except that they
must be called with the same template parameter and the same bus width.

The functions manage their own memory.  Whatever memory they dynamically
allocate, they also liberate, unless they use pooling for efficiency.

All functions assume power-of-2 bus and data word widths.

Byte-enables are supported only when byte-enable granularity is no finer than
the data word (every data word is either wholly enabled or wholly disabled)
and only when the byte-enable-length is equal to the data length.  The
reasons for this are:
- repeating byte-enable patterns are likely to benefit from tailored
endianness conversion functions
- it is in any case appropriate to provide conversion functions which
don't support such repeated byte-enable patterns, because of the efficiency
saving in the inner loops


Functions offered:

1) A pair of functions that work for all transactions regardless of data and
bus data sizes and address alignment, but subject to the limitations listed
above.  Always allocates a new data buffer and
a new byte enable buffer.  Byte enables are assumed to be needed even if not
required for the original (unconverted) transaction.  Data is copied to the
new buffer on request (for writes) or on response (for reads).
  to_hostendian_generic(tlm_generic_payload *txn, int sizeof_databus)
  from_hostendian_generic(tlm_generic_payload *txn, int sizeof_databus)

2) If the original transaction is both word and bus-aligned (an
integer multiple of both data words and bus words, with data word
equal to or smaller than the bus word) then this pair of
functions can be used.  It will complete faster than the generic function
because the data reordering function is much simpler and no address
conversion is required.
  to_hostendian_aligned(tlm_generic_payload *txn, int sizeof_databus)
  from_hostendian_aligned(tlm_generic_payload *txn, int sizeof_databus)

3) For single word transactions that don't cross a bus word boundary it
is always safe to work in-place and the conversion is very simple.
  to_hostendian_single(tlm_generic_payload *txn, int sizeof_databus)
  from_hostendian_single(tlm_generic_payload *txn, int sizeof_databus)
*/


#define uchar unsigned char


///////////////////////////////////////////////////////////////////////////////
// Generic Utilities

// a pool for uchar* buffers of arbitrary but bounded size.  the pool contains
// buffers with a fixed size - the largest so far requested.
class buffer_pool {
  int max_buffer_size;
  uchar* pool_head;

  public:
    buffer_pool(): max_buffer_size(32), pool_head(0) {};

    uchar *get_a_buffer(int size) {
      if(size > max_buffer_size) {
        max_buffer_size = size;
        // empty the pool - it will have to grow again naturally
        for(uchar *p = pool_head; p != 0; ) {
          uchar *q = p;
          p = *((uchar **)(p + sizeof(int)));
          delete [] q;
        }
        pool_head = 0;
      }
      if(pool_head == 0) {
        // do a real malloc because pool is empty
        // allocate 2 spare spaces, one for the size and the other for the
        // next-pointer
        pool_head = new uchar[max_buffer_size + sizeof(int) + sizeof(uchar *)];
        *((int *)pool_head) = max_buffer_size;
        *((uchar **)(pool_head + sizeof(int))) = 0;
      }
      // now pop the pool and return the old head
      uchar *retval = pool_head + sizeof(int) + sizeof(uchar *);
      pool_head = *((uchar **)(pool_head + sizeof(int)));
      return retval;
    };

    void return_buffer(uchar *p) {
      // calculate the start of the actual buffer
      uchar *q = p - sizeof(int) - sizeof(uchar *);
      if(*((int *)q) != max_buffer_size) {
        // this buffer's size is out of date.  throw it away
        delete [] q;
      } else {
        // push a buffer into the pool if it has the right size
        *((uchar **)(q + sizeof(int))) = pool_head;
        pool_head = q;
      }
    }
};

static buffer_pool local_buffer_pool;

// an extension to keep the information needed for reconversion of response
class txn_endian_context : public tlm_extension<txn_endian_context> {
  public:
    sc_dt::uint64 address;     // used by generic
    uchar *data_ptr;     // used by generic and aligned
    uchar *byte_enable;  // used by generic
    int length;         // used by generic
    // for pooling
    txn_endian_context *next;
    tlm_extension_base* clone() const {return 0;}
    void anti_clone() {return;}
};
// Assumptions about transaction contexts:
// 1) only the address attribute of a transaction
// is mutable.  all other attributes are unchanged from the request to
// response side conversion.
// 2) the conversion functions in this file do not respect the mutability
// rules and do not put the transaction back into its original state after
// completion.  so if the initiator has any cleaning up to do (eg of byte
// enable buffers), it needs to store its own context.  the transaction
// returned to the initiator may contain pointers to data and byte enable
// that can/must not be deleted.

// a pool for transaction context objects
static txn_endian_context *free_contexts = 0;

static txn_endian_context *get_a_context() {
  if(free_contexts == 0) {
    return new txn_endian_context;
  }
  txn_endian_context *ret = free_contexts;
  free_contexts = free_contexts->next;
  return ret;
}

static void return_context(txn_endian_context *tc) {
  tc->next = free_contexts;
  free_contexts = tc;
}

// a set of constants for efficient filling of byte enables
template<class D> class tlm_bool {
  public:
    static D TLM_TRUE;
    static D TLM_FALSE;
    static D make_uchar_array(uchar c) {
      uchar *tmp = new uchar[sizeof(D)];
      for(unsigned int i=0; i<sizeof(D); i++) tmp[i] = c;
      return *((D *)tmp);
    }
};

template<class D> D tlm_bool<D>::TLM_TRUE = tlm_bool<D>::make_uchar_array(1);
template<class D> D tlm_bool<D>::TLM_FALSE = tlm_bool<D>::make_uchar_array(0);



///////////////////////////////////////////////////////////////////////////////
// function set (1): Utilities
template<class D>
inline void copy_d1(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2) {
  *((D *)dest1) = *((D *)src1);
  *((D *)dest2) = tlm_bool<D>::TLM_TRUE;
}

template<class D>
inline void copy_db1(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2) {
  *((D *)dest1) = *((D *)src1);
  *((D *)dest2) = *((D *)src2);
}

template<class D>
inline void true_b1(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2) {
  *((D *)dest2) = tlm_bool<D>::TLM_TRUE;
}

template<class D>
inline void copy_b1(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2) {
  *((D *)dest2) = *((D *)src2);
}

template<class D>
inline void copy_dbyb1(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2) {
  if(*src2 != 0)  *((D *)src1) = *((D *)dest1);
}

template<class D>
inline void copy_dbytrue1(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2) {
  *((D *)src1) = *((D *)dest1);
}

template<class D> inline void false_b1(uchar *dest1) {
  *((D *)dest1) = tlm_bool<D>::TLM_FALSE;
}

template<class D> inline void no_b1(uchar *dest1) {
}

template<class D,
         void COPY(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2),
         void COPYuchar(uchar *src1, uchar *src2, uchar *dest1, uchar *dest2),
         void FILLFALSE(uchar *dest1), void FILLFALSEuchar(uchar *dest1)>
inline int loop_generic1(
  int bytes_left, int len0, int lenN, int sizeof_databus,
  uchar *start, uchar *end, uchar *src, uchar *bsrc, uchar *dest, uchar *bdest) {
  int d2b_src = bsrc - src;
  int d2b_dest = bdest - dest;
  uchar *original_dest = dest;

  while(true) {
    // len0 bytes at start of a bus word
    if((src >= start) && (src < end)) {
      for(int i=0; i<len0; i++) {
        COPYuchar(src, src+d2b_src, dest, dest+d2b_dest);
        src++;
        dest++;
      }
      bytes_left -= len0;
      if(bytes_left <= 0) return int(dest - original_dest);
    } else {
      for(int i=0; i<len0; i++) {
        FILLFALSEuchar(dest+d2b_dest);
        src++;
        dest++;
      }
    }
    src -= 2 * sizeof(D);

    // sequence of full data word fragments
    for(unsigned int i=1; i<sizeof_databus/sizeof(D); i++) {
      if((src >= start) && (src < end)) {
        COPY(src, src+d2b_src, dest, dest+d2b_dest);
        bytes_left -= sizeof(D);
      } else {
        FILLFALSE(dest+d2b_dest);
      }
      dest += sizeof(D);
      if(bytes_left <= 0) return int(dest - original_dest);
      src -= sizeof(D);
    }

    // lenN bytes at end of bus word
    if((src >= start) && (src < end)) {
      for(int i=0; i<lenN; i++) {
        COPYuchar(src, src+d2b_src, dest, dest+d2b_dest);
        src++;
        dest++;
      }
      bytes_left -= lenN;
      if(bytes_left <= 0) return int(dest - original_dest);
    } else {
      for(int i=0; i<lenN; i++) {
        FILLFALSEuchar(dest+d2b_dest);
        src++;
        dest++;
      }
    }
    src += 2 * sizeof_databus;
  }
}


///////////////////////////////////////////////////////////////////////////////
// function set (1): Request
template<class DATAWORD> inline void
to_hostendian_generic(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  sc_dt::uint64 b_mask = sizeof_databus - 1;
  int d_mask = sizeof(DATAWORD) - 1;
  sc_dt::uint64 a_aligned = txn->get_address() & ~b_mask;
  int a_offset = txn->get_address() & b_mask;
  int len0 = (sizeof_databus - a_offset) & d_mask;
  int lenN = sizeof(DATAWORD) - len0;
  uchar *d_start = txn->get_data_ptr();
  uchar *d_end = txn->get_data_length() + d_start;
  uchar *d = ((sizeof_databus - a_offset) & ~d_mask) + lenN + d_start;

  if(sizeof(DATAWORD) > sizeof_databus) exit(1);  // FIXME //

  // create new data and byte enable buffers
  int long_enough = txn->get_data_length() + 2 * sizeof_databus;
  uchar *new_data = local_buffer_pool.get_a_buffer(long_enough);
  uchar *new_be = local_buffer_pool.get_a_buffer(long_enough);

  if(txn->is_read()) {
    txn_endian_context *tc = get_a_context();
    tc->data_ptr = d_start;
    tc->address = txn->get_address();
    tc->byte_enable = txn->get_byte_enable_ptr();
    tc->length = txn->get_data_length();
    txn->set_extension(tc);
    if(txn->get_byte_enable_ptr() == 0) {
      // iterate over transaction creating new byte enables from all-true
      txn->set_data_length(loop_generic1<DATAWORD, &true_b1<DATAWORD>,
        &true_b1<uchar>, &false_b1<DATAWORD>, &false_b1<uchar> >(
        txn->get_data_length(), len0, lenN, sizeof_databus,
        d_start, d_end, d, 0, new_data, new_be));
    } else {
      // iterate over transaction copying byte enables
      txn->set_data_length(loop_generic1<DATAWORD, &copy_b1<DATAWORD>,
        &copy_b1<uchar>, &false_b1<DATAWORD>, &false_b1<uchar> >(
        txn->get_data_length(), len0, lenN, sizeof_databus, d_start, d_end,
        d, txn->get_byte_enable_ptr() - d_start + d, new_data, new_be));
    }
  } else {
    // WRITE
    if(txn->get_byte_enable_ptr() == 0) {
      // iterate over transaction copying data and creating new byte-enables
      txn->set_data_length(loop_generic1<DATAWORD, &copy_d1<DATAWORD>,
        &copy_d1<uchar>, &false_b1<DATAWORD>, &false_b1<uchar> >(
        txn->get_data_length(), len0, lenN, sizeof_databus,
        d_start, d_end, d, 0, new_data, new_be));
    } else {
      // iterate over transaction copying data and byte-enables
      txn->set_data_length(loop_generic1<DATAWORD, &copy_db1<DATAWORD>,
        &copy_db1<uchar>, &false_b1<DATAWORD>, &false_b1<uchar> >(
        txn->get_data_length(), len0, lenN, sizeof_databus, d_start, d_end,
        d, txn->get_byte_enable_ptr() - d_start + d, new_data, new_be));
    }
  }
  txn->set_data_ptr(new_data);
  txn->set_byte_enable_ptr(new_be);
  txn->set_address(a_aligned);
}


///////////////////////////////////////////////////////////////////////////////
// function set (1): Response
template<class DATAWORD> inline void
from_hostendian_generic(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  if(txn->is_read()) {
    txn_endian_context *tc;
    txn->get_extension(tc);

    sc_dt::uint64 b_mask = sizeof_databus - 1;
    int d_mask = sizeof(DATAWORD) - 1;
    int a_offset = tc->address & b_mask;
    int len0 = (sizeof_databus - a_offset) & d_mask;
    int lenN = sizeof(DATAWORD) - len0;
    uchar *d_start = tc->data_ptr;
    uchar *d_end = tc->length + d_start;
    uchar *d = ((sizeof_databus - a_offset) & ~d_mask) + lenN + d_start;

    // iterate over transaction copying data qualified by byte-enables
    if(tc->byte_enable == 0) {
      loop_generic1<DATAWORD, &copy_dbytrue1<DATAWORD>,
        &copy_dbytrue1<uchar>, &no_b1<DATAWORD>, &no_b1<uchar> >(
        tc->length, len0, lenN, sizeof_databus, d_start, d_end, d,
        0, txn->get_data_ptr(), 0);
    } else {
      loop_generic1<DATAWORD, &copy_dbyb1<DATAWORD>,
        &copy_dbyb1<uchar>, &no_b1<DATAWORD>, &no_b1<uchar> >(
        tc->length, len0, lenN, sizeof_databus, d_start, d_end, d,
        tc->byte_enable - d_start + d, txn->get_data_ptr(), 0);
    }

    return_context(tc);
  }
  local_buffer_pool.return_buffer(txn->get_byte_enable_ptr());
  local_buffer_pool.return_buffer(txn->get_data_ptr());
}



///////////////////////////////////////////////////////////////////////////////
// function set (2): Utilities
template<class D> inline void copy_d2(D *src1, D *src2, D *dest1, D *dest2) {
  *dest1 = *src1;
}

template<class D> inline void copy_db2(D *src1, D *src2, D *dest1, D *dest2) {
  *dest1 = *src1;
  *dest2 = *src2;
}

template<class D>
inline void copy_dbyb2(D *src1, D *src2, D *dest1, D *dest2) {
  if(*((uchar *)src2) != 0) *dest1 = *src1;
}

template<class D, void COPY(D *src1, D *src2, D *dest1, D *dest2)>
inline void loop_aligned2(D *src1, D *src2, D *dest1, D *dest2,
    int words, int words_per_bus) {
  int src1to2 = int(src2) - int(src1);
  int dest1to2 = int(dest2) - int(dest1);

  D *done = src1 + words;
  D *bus_start = src1;
  src1 += words_per_bus - 1;

  while(true) {
    COPY(src1, (D *)(int(src1)+src1to2), dest1, (D *)(int(dest1)+dest1to2));
    dest1++;
    if((--src1) < bus_start) {
      bus_start += words_per_bus;
      if(bus_start == done) break;
      src1 = bus_start + words_per_bus - 1;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
// function set (2): Request
template<class DATAWORD> inline void
to_hostendian_aligned(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  int words_per_bus = sizeof_databus/sizeof(DATAWORD);
  if(words_per_bus == 1) return;
  int words = (txn->get_data_length())/sizeof(DATAWORD);

  DATAWORD *original_be = (DATAWORD *)(txn->get_byte_enable_ptr());
  DATAWORD *original_data = (DATAWORD *)(txn->get_data_ptr());

  // always allocate a new data buffer
  txn->set_data_ptr(local_buffer_pool.get_a_buffer(txn->get_data_length()));

  if(original_be == 0) {
    // no byte enables
    if(txn->is_write()) {
      // WR no byte enables.  Copy data
      loop_aligned2<DATAWORD, &copy_d2<DATAWORD> >(original_data, 0,
        (DATAWORD *)(txn->get_data_ptr()), 0,
        words, words_per_bus);
    } else {
      // RD no byte enables.  Save original data pointer
      txn_endian_context *tc = get_a_context();
      tc->data_ptr = (uchar *)original_data;
      txn->set_extension(tc);
    }
  } else {
    // byte enables present
    // allocate a new buffer for them
    txn->set_byte_enable_ptr(
      local_buffer_pool.get_a_buffer(txn->get_data_length()));

    if(txn->is_write()) {
      // WR with byte enables.  Copy data and BEs
      loop_aligned2<DATAWORD, &copy_db2<DATAWORD> >(original_data, original_be,
        (DATAWORD *)(txn->get_data_ptr()),
        (DATAWORD *)(txn->get_byte_enable_ptr()), words, words_per_bus);
    } else {
      // RD with byte enables.  Save original data pointer
      txn_endian_context *tc = get_a_context();
      tc->data_ptr = (uchar *)original_data;
      txn->set_extension(tc);
      // Copy byte enables to new buffer
      loop_aligned2<DATAWORD, &copy_d2<DATAWORD> >(original_be, 0,
        (DATAWORD *)(txn->get_byte_enable_ptr()), 0,
        words, words_per_bus);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
// function set (2): Response
template<class DATAWORD> inline void
from_hostendian_aligned(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  int words_per_bus = sizeof_databus/sizeof(DATAWORD);
  if(words_per_bus == 1) return;
  int words = (txn->get_data_length())/sizeof(DATAWORD);

  if(txn->get_byte_enable_ptr() == 0) {
    // no byte enables
    if(txn->is_read()) {
      // RD without byte enables.  Copy data to original buffer
      txn_endian_context *tc;
      txn->get_extension(tc);
      loop_aligned2<DATAWORD, &copy_d2<DATAWORD> >(
        (DATAWORD *)(txn->get_data_ptr()),
        0, (DATAWORD *)(tc->data_ptr), 0, words, words_per_bus);
      return_context(tc);
    }
  } else {
    // byte enables present
    if(txn->is_read()) {
      // RD with byte enables.  Copy data qualified by byte-enables
      txn_endian_context *tc;
      txn->get_extension(tc);
      loop_aligned2<DATAWORD, &copy_dbyb2<DATAWORD> >(
        (DATAWORD *)(txn->get_data_ptr()),
        (DATAWORD *)(txn->get_byte_enable_ptr()),
        (DATAWORD *)(tc->data_ptr), 0, words, words_per_bus);
      return_context(tc);
    }
    local_buffer_pool.return_buffer(txn->get_byte_enable_ptr());
  }
  // in all cases free the new data buffer
  local_buffer_pool.return_buffer(txn->get_data_ptr());
}



///////////////////////////////////////////////////////////////////////////////
// function set (3): Request
template<class DATAWORD> inline void
to_hostendian_single(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  // only need to change the address, always safe to work in-place
  sc_dt::uint64 mask = sizeof_databus-1;
  sc_dt::uint64 a = txn->get_address();
  txn->set_address((a & ~mask) |
    (sizeof_databus - (a & mask) - sizeof(DATAWORD)));
}


///////////////////////////////////////////////////////////////////////////////
// function set (3): Response
template<class DATAWORD> inline void
from_hostendian_single(tlm_generic_payload *txn, unsigned int sizeof_databus) {
  // nothing needs to be done here
}


#undef uchar


}  // namespace tlm


#endif  // multiple-inclusion protection

