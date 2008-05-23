"""
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

Python Script to test Endianness Conversion Functions for OSCI TLM-2

TO DO:
Make it faster.  (1) don't calculate a new memory state for every
transaction.  One every 10 is enough, seeing as everything random
anyway.  (2) don't keep starting new SystemC processes, but just pipe
transactions to the same one which contains a loop.

There is a simple testbench programme in C++ which runs a single
transaction through a single conversion function, to a simple target
memory and back.  This script will run the testbench many times and test for
- incomplete execution, seg-faults, etc
- distributability of conversion function: each transaction should have
the same effect on initiator/target memory as a set of smaller transactions
that sum to it
- equivalence: all conversion functions should have the same functional
effect as each other

The approach is to provide the initial state of the initiator and
target memory (as strings) and to capture their final states, so that
only the effect of the transaction on the data buffers is measured.

Script works out for itself which conversion functions are legal for a
given transaction and applies all of them.  Note that where data is
wider than bus, only one conversion function is legal and testing is
somewhat limited.

Testing space (select a transaction at random from the space):
- with and without byte-enables (generated at random for each data word
and can be all-zero)
- data widths of (1,2,4,8,16)
- bus widths of (1,2,4,8,16), lower priority for those smaller than data
width
- transaction lengths of (1..32) x data width, higher probability for
lower values
- base address (0..1023) at bus_width steps
- offset address (0..bus width) with a higher priority for 0
- address in initiator buffer uniform random
- read or write
- byte-enable length may be smaller than transasction length
- may be a streaming burst

Transaction breakdown
- individual words (always)
- one random breakdown with each segment containing between 1 and N-1
words, where N is the length in words
- one breakdown with two segments interleaved, using (additional) byte
enables to suppress the part where the other segment is active

Data buffer definition:  starts at 0, randomly filled
with lower case letters and numbers.  Size 2 kB.  Addresses are limited to
1 kB.
"""


import random
import string


class transaction:
  """ contains read_not_write, address, length, byte_enable,
      bus_width, data_width, data_pointer, stream_width """
  def __init__(self, **a):  self.__dict__ = a
  def __str__(self):
    if self.read_not_write:  a = "R: "
    else:  a = "W: "
    a += "addr = %d, len = %d, bus = %d, word = %d, data = %d" % \
      (self.address, self.length, self.bus_width, self.data_width, \
       self.data_pointer)
    if self.byte_enable:  a += ", be = " + self.byte_enable
    else:  a += ", be = x"
    a += ", sw = %d" % (self.stream_width)
    return a


def txn_generator(nr):
  pr_read = 0.5
  pr_byte_enable = 0.5
  pr_enabled = 0.5
  bus_widths = [1, 2, 4, 8, 16]
  data_widths = [1, 2, 4, 8, 16] + [1, 2, 4, 8] + [1, 2, 4] + [1, 2]
  lengths = range(1,33) + range(1,17) + range(1,9) + range(1,5) + range(1,3)
  pr_short_be = 0.2
  pr_stream = 0.1
  nr_generated = 0
  while nr_generated < nr:
    # create a random transaction
    bus_width = random.choice(bus_widths)
    while True:
      data_width = random.choice(data_widths)
      if data_width <= bus_width:  break
      if random.random() < 0.25:  break
    length = random.choice(lengths)
    addr_base = random.choice(range(0,1024,bus_width))
    addr_offset = random.choice(range(bus_width)+[0]*(bus_width/2))
    txn = transaction(
      bus_width = bus_width,
      data_width = data_width,
      read_not_write = random.random() < pr_read,
      length = length * data_width,
      address = addr_base + addr_offset,
      byte_enable = False,
      stream_width = length * data_width,
      data_pointer = random.randint(0,1023)
    )
    if random.random() < pr_byte_enable:
      belen = length
      if random.random() < pr_short_be:
        belen = min(random.choice(lengths), length)
      bep = ["0" * data_width, "1" * data_width]
      txn.byte_enable = "".join([random.choice(bep) for x in range(belen)])
    if random.random() < pr_stream and length > 1:
      strlen = length
      while True:
        strlen -= 1
        if strlen == 1 or \
          (random.random() < 0.5 and (length/strlen)*strlen == length):
          break
      txn.stream_width = strlen * data_width
    nr_generated += 1
    yield txn

# test code for transaction generator
if False:
  for t in txn_generator(20):
    print t
  raise Exception
# end test code


class memory_state_cl:
  buffer_size = 2048
  repeats = 10 * buffer_size / 36
  population = (string.lowercase + string.digits) * repeats
  def __init__(self):
    self.initiator = "".join(
      random.sample(memory_state_cl.population, memory_state_cl.buffer_size))
    self.target =  "".join(
      random.sample(memory_state_cl.population, memory_state_cl.buffer_size))
  def copy(self):
    r = memory_state_cl()
    r.initiator = self.initiator
    r.target = self.target
    return r
  def __eq__(self, golden):
    return self.initiator==golden.initiator and self.target==golden.target
  def __ne__(self, golden):
    return self.initiator!=golden.initiator or self.target!=golden.target
  def __str__(self):
    return "initiator = " + self.initiator + "\n" + "target = " + self.target


# all fragmentation generators
def __FRAG__null(txn):
  yield txn

def __FRAG__word(txn):
  curr_address = txn.address
  reset_address = curr_address + txn.stream_width
  if txn.byte_enable:
    full_byte_enable = txn.byte_enable * (1+txn.length/len(txn.byte_enable))
  be_pos = 0
  d_pos = txn.data_pointer
  end = txn.length + d_pos
  while d_pos < end:
    new_txn = transaction(
      bus_width = txn.bus_width,
      data_width = txn.data_width,
      read_not_write = txn.read_not_write,
      length = txn.data_width,
      address = curr_address,
      byte_enable = False,
      stream_width = txn.data_width,
      data_pointer = d_pos
    )
    curr_address += txn.data_width
    if curr_address == reset_address:  curr_address = txn.address
    d_pos += txn.data_width
    if txn.byte_enable:
      new_txn.byte_enable = full_byte_enable[be_pos:be_pos+txn.data_width]
      be_pos += txn.data_width
    yield new_txn

def __FRAG__stream(txn):
  if txn.byte_enable:
    full_byte_enable = txn.byte_enable * (1+txn.length/len(txn.byte_enable))
  be_pos = 0
  bytes_done = 0
  while bytes_done < txn.length:
    new_txn = transaction(
      bus_width = txn.bus_width,
      data_width = txn.data_width,
      read_not_write = txn.read_not_write,
      length = txn.stream_width,
      address = txn.address,
      byte_enable = False,
      stream_width = txn.stream_width,
      data_pointer = bytes_done + txn.data_pointer
    )
    if txn.byte_enable:
      new_txn.byte_enable = full_byte_enable[be_pos:be_pos+txn.stream_width]
      be_pos += txn.stream_width
    yield new_txn
    bytes_done += txn.stream_width

def __FRAG__random(stream_txn):
  for txn in __FRAG__stream(stream_txn):
    # txn has full byte enables and no stream feature guaranteed
    pr_nofrag = 0.5
    end_address = txn.address + txn.length
    curr_address = txn.address
    be_pos = 0
    d_pos = txn.data_pointer
    while curr_address < end_address:
      new_txn = transaction(
        bus_width = txn.bus_width,
        data_width = txn.data_width,
        read_not_write = txn.read_not_write,
        length = txn.data_width,
        address = curr_address,
        byte_enable = txn.byte_enable,
        stream_width = txn.data_width,
        data_pointer = d_pos
      )
      curr_address += txn.data_width
      d_pos += txn.data_width
      if txn.byte_enable:
        new_txn.byte_enable = txn.byte_enable[be_pos:be_pos+txn.data_width]
        be_pos += txn.data_width
      while random.random() < pr_nofrag and curr_address < end_address:
        new_txn.length += txn.data_width
        new_txn.stream_width += txn.data_width
        curr_address += txn.data_width
        d_pos += txn.data_width
        if txn.byte_enable:
          new_txn.byte_enable += txn.byte_enable[be_pos:be_pos+txn.data_width]
          be_pos += txn.data_width
      yield new_txn

def __FRAG__randinterleave(stream_txn):
  for txn in __FRAG__stream(stream_txn):
    # txn has full byte enables and no stream feature guaranteed
    pr_frag = 0.5
    txns = [ transaction(
      bus_width = txn.bus_width,
      data_width = txn.data_width,
      read_not_write = txn.read_not_write,
      length = txn.length,
      address = txn.address,
      byte_enable = "",
      stream_width = txn.length,
      data_pointer = txn.data_pointer
    ), transaction(
      bus_width = txn.bus_width,
      data_width = txn.data_width,
      read_not_write = txn.read_not_write,
      length = txn.length,
      address = txn.address,
      byte_enable = "",
      stream_width = txn.length,
      data_pointer = txn.data_pointer
    ) ]
    curr = 0
    be_pos = 0
    on = "1" * txn.data_width
    off = "0" * txn.data_width
    while be_pos < txn.length:
      if txn.byte_enable:  bew = txn.byte_enable[be_pos:be_pos+txn.data_width]
      else:  bew = on
      txns[curr].byte_enable += bew
      txns[1-curr].byte_enable += off
      be_pos += txn.data_width
      if random.random() < pr_frag:  curr = 1-curr
    yield txns[0]
    yield txns[1]

fragmenters = \
  [__FRAG__null, __FRAG__word, __FRAG__stream, __FRAG__random, __FRAG__randinterleave]

# test code for fragmenters
if False:
  for t in txn_generator(1):
    print t
    print
    for u in fragmenters[4](t):
      print u
  raise Exception
# end test code



# conversion functions are determined by an index (shared with C++) and
# a function that tests if they can be applied to a transaction
def check_generic(txn):
  return True

def check_word(txn):
  if txn.data_width > txn.bus_width:  return False
  if txn.stream_width < txn.length:  return False
  if txn.byte_enable and len(txn.byte_enable) < txn.length:  return False
  return True

def check_aligned(txn):
  if txn.data_width > txn.bus_width:  return False
  if txn.stream_width < txn.length:  return False
  if txn.byte_enable and len(txn.byte_enable) < txn.length:  return False
  base_addr = txn.address / txn.bus_width
  if base_addr * txn.bus_width != txn.address:  return False
  nr_bus_words = txn.length / txn.bus_width
  if nr_bus_words * txn.bus_width != txn.length:  return False
  return True

def check_single(txn):
  if txn.length != txn.data_width:  return False
  base_addr = txn.address / txn.bus_width
  end_base_addr = (txn.address + txn.length) / txn.bus_width
  if base_addr != end_base_addr:  return False
  return True

def check_local_single(txn):
  if txn.length != txn.data_width:  return False
  return True

all_converters = [check_generic, check_word, check_aligned, check_single, check_local_single]
usage = [0 for x in all_converters]


class SystemCFailure(Exception):  pass
class ConverterDifference(Exception):  pass
class FragmenterDifference(Exception):  pass

from subprocess import Popen, PIPE

# test a single fragment one way
def run_test(fragment, memstate, converter):
  txtin = "%s\n%s\nconverter = %d\n" % (fragment, memstate, converter)
  txtout = "no output"
  try:
    sp = Popen("../build-unix/test_endian_conv.exe", stdin=PIPE, stdout=PIPE)
    txtout = sp.communicate(txtin)[0]
    result = memory_state_cl()
    tmp = txtout.splitlines()
    result.initiator = [l.split()[-1] for l in tmp if "initiator =" in l][-1]
    result.target = [l.split()[-1] for l in tmp if "target =" in l][-1]
  except:
    raise SystemCFailure("\n" + txtin + txtout)
  if sp.returncode != 0:  raise SystemCFailure("\n" + txtin + txtout)
  usage[converter] += 1
  return result


# test a single fragment in multiple ways
def test_a_fragment(f, ms):
  # f is the (fragment of a) transaction
  # ms is the memory state to use at start of test

  # run the same fragment through all applicable conversion functions
  # and check they all do the same thing
  first_run = True
  for i,checker in enumerate(all_converters):
    if checker(f):
      ms_out = run_test(f, ms, i)

      if first_run:
        golden_ms = ms_out.copy()
        golden_idx = i
        first_run = False
      else:
        if ms_out != golden_ms:  raise ConverterDifference("""
%s
start memory:
%s
converter = %d
golden memory:
%s
actual memory:
%s""" % (f, ms, i, golden_ms, ms_out))

  return ms_out


# main loop

from sys import argv

print "Testing Endianness Conversion Functions"
print "March 2008"
print "OSCI TLM-2"

try:  nr_txns_to_test = int(argv[1])
except:
  print "No command line input for number of tests, using default"
  nr_txns_to_test = 1000

print "Number to test:", nr_txns_to_test

# generate and test a number of transactions
for txn in txn_generator(nr_txns_to_test):

  # each transaction has a random initial memory state
  initial_memory = memory_state_cl()

  # iterate over all defined fragmentation functions
  first_time = True
  for fragmenter in fragmenters:

    # all versions of the transaction start in the same place
    memory_state = initial_memory.copy()

    # now iterate over the fragments of the transaction, accumulating
    # the memory state
    for partial_txn in fragmenter(txn):
      memory_state = test_a_fragment(partial_txn, memory_state)

    if first_time:
      golden_memory_state = memory_state.copy()
      first_time = False
    else:
      if memory_state != golden_memory_state:  raise FragmenterDifference("""
fragmenter: %s
transaction:
%s
start memory:
%s
golden memory:
%s
actual memory:
%s""" % (fragmenter, txn, initial_memory, golden_memory_state, memory_state))

  print ".",
print

print "Conversion functions usage frequency:"
print "  generic", usage[0]
print "     word", usage[1]
print "  aligned", usage[2]
print "   single", usage[3]
print " local single", usage[4]

