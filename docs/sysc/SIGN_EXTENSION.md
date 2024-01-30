Sign Extension of SystemC Data Types
====================================

This document describes how sign extension is done for SystemC data types. This
includes implicit use of those data types, as well as methods like to_int()

The Proof Of Concept simulator is configured to use the following rules:

* Values that are smaller than a required width will be sign extended to the proper width. 

* Signed values will "sign extend" the upper bit of the value to extend to the proper width.

* Unsigned values will "zero extend" a zero bit to extend to the proper width.

These values are signed:

    sc_bigint
    sc_signed
    sc_int
    sc_int_base
    C++ signed integer types, e.g., int, long long

These values are unsigned (includes all the SystemC bit vector types):

    sc_biguint
    sc_unsigned
    sc_uint
    sc_uint_base
    sc_bv
    sc_bv_base
    sc_lv
    sc_lv_base
    sc_signed_subref
    sc_signed_subref_r
    sc_unsigned_subref
    sc_unsigned_subref_r
    sc_int_subref
    sc_int_subref_r
    sc_uint_subref
    sc_uint_subref_r
    C++ unsigned integer types, e.g., unsigned int, unsigned long

These types are bool:

    sc_signed_bitref
    sc_signed_bitref_r
    sc_unsigned_bitref
    sc_unsigned_bitref_r
    sc_int_bitref
    sc_int_bitref_r
    sc_uint_bitref
    sc_uint_bitref_r
    C++ bool type

The sign extension rules are enforced for the to_XXXX methods associated with
the various SystemC types:

    to_int()
    to_uint()
    to_long()
    to_ulong()
    to_int64()
    to_uint64()

Signed SystemC datatypes will sign extend their values when creating the return
type for these methods, For instance, given:

    sc_int<10> x;
    unsigned int j;
    .  .  .
    j = x.to_uint();

x will be sign extended using its upper bit to a width of 32 bits, and that
value assigned to j.

Similarly, to_long for an unsigned type like sc_biguint<12> will be zero
extended to the width of a C++ long, and that value returned.
