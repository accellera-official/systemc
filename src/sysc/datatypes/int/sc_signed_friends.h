/*****************************************************************************
  
  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

// THIS IS THE INCLUSION FILE FOR FRIENDS OF THE sc_signed CLASS. 
//
//(1) This file gets included twice in sc_signed.h, once with SCFP const set to 
//     null and once with SCFP set to "friend". So there is no guard
//     around its contents.
//(2) 

// operator + with an sc_signed result:

SCFP const sc_signed   operator+(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator+(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator+(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator+(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator+(long                u, const sc_signed&    v);
SCFP const sc_signed   operator+(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator+(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator+(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator+(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator+(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator+(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator+(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator+(const sc_signed&    u, long                v);
SCFP const sc_signed   operator+(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator+(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator+(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator+(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator+(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator+(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator+(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator+(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator+(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator+(int64               u, const sc_unsigned&  v);

SCFP const sc_signed   operator+(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator+(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator+(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator+(const sc_unsigned&  u, int64               v);

// operator * with an sc_signed result:

SCFP const sc_signed   operator*(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator*(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator*(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator*(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator*(long                u, const sc_signed&    v);
SCFP const sc_signed   operator*(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator*(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator*(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator*(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator*(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator*(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator*(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator*(const sc_signed&    u, long                v);
SCFP const sc_signed   operator*(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator*(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator*(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator*(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator*(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator*(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator*(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator*(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator*(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator*(int64               u, const sc_unsigned&  v);

SCFP const sc_signed   operator*(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator*(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator*(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator*(const sc_unsigned&  u, int64               v);

// operator / with an sc_signed result:

SCFP const sc_signed   operator/(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator/(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator/(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator/(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator/(long                u, const sc_signed&    v);
SCFP const sc_signed   operator/(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator/(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator/(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator/(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator/(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator/(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator/(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator/(const sc_signed&    u, long                v);
SCFP const sc_signed   operator/(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator/(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator/(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator/(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator/(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator/(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator/(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator/(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator/(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator/(int64               u, const sc_unsigned&  v);

SCFP const sc_signed   operator/(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator/(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator/(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator/(const sc_unsigned&  u, int64               v);

// operator % with an sc_signed result:

SCFP const sc_signed   operator%(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator%(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator%(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator%(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator%(long                u, const sc_signed&    v);
SCFP const sc_signed   operator%(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator%(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator%(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator%(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator%(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator%(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator%(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator%(const sc_signed&    u, long                v);
SCFP const sc_signed   operator%(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator%(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator%(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator%(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator%(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator%(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator%(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator%(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator%(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator%(int64               u, const sc_unsigned&  v);

SCFP const sc_signed   operator%(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator%(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator%(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator%(const sc_unsigned&  u, int64               v);

// operator - with an sc_signed result:

SCFP const sc_signed   operator-(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator-(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator-(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator-(long                u, const sc_signed&    v);
SCFP const sc_signed   operator-(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator-(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator-(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator-(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator-(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator-(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator-(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator-(const sc_signed&    u, long                v);
SCFP const sc_signed   operator-(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator-(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator-(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator-(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator-(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator-(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(int64               u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(const sc_uint_base& u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(unsigned int        u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(uint64              u, const sc_unsigned&  v);
SCFP const sc_signed   operator-(unsigned long       u, const sc_unsigned&  v);

SCFP const sc_signed   operator-(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, int64               v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, const sc_uint_base& v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, unsigned int        v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, uint64              v);
SCFP const sc_signed   operator-(const sc_unsigned&  u, unsigned long       v);

// operator & with an sc_signed result:

SCFP const sc_signed   operator&(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator&(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator&(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator&(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator&(long                u, const sc_signed&    v);
SCFP const sc_signed   operator&(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator&(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator&(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator&(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator&(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator&(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator&(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator&(const sc_signed&    u, long                v);
SCFP const sc_signed   operator&(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator&(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator&(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator&(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator&(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator&(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator&(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator&(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator&(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator&(int64               u, const sc_unsigned&  v);

SCFP const sc_signed   operator&(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator&(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator&(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator&(const sc_unsigned&  u, int64               v);

// operator | with an sc_signed result:

SCFP const sc_signed   operator|(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator|(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator|(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator|(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator|(long                u, const sc_signed&    v);
SCFP const sc_signed   operator|(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator|(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator|(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator|(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator|(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator|(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator|(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator|(const sc_signed&    u, long                v);
SCFP const sc_signed   operator|(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator|(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator|(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator|(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator|(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator|(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator|(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator|(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator|(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator|(int64               u, const sc_unsigned&  v);

SCFP const sc_signed   operator|(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator|(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator|(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator|(const sc_unsigned&  u, int64               v);

// operator ^ with an sc_signed result:

SCFP const sc_signed   operator^(const sc_signed&    u, const sc_signed&    v);
SCFP const sc_signed   operator^(const sc_unsigned&  u, const sc_signed&    v);
SCFP const sc_signed   operator^(const sc_int_base&  u, const sc_signed&    v);
SCFP const sc_signed   operator^(int                 u, const sc_signed&    v);
SCFP const sc_signed   operator^(long                u, const sc_signed&    v);
SCFP const sc_signed   operator^(int64               u, const sc_signed&    v);
SCFP const sc_signed   operator^(const sc_uint_base& u, const sc_signed&    v);
SCFP const sc_signed   operator^(unsigned int        u, const sc_signed&    v);
SCFP const sc_signed   operator^(uint64              u, const sc_signed&    v);
SCFP const sc_signed   operator^(unsigned long       u, const sc_signed&    v);

SCFP const sc_signed   operator^(const sc_signed&    u, const sc_int_base&  v);
SCFP const sc_signed   operator^(const sc_signed&    u, int                 v);
SCFP const sc_signed   operator^(const sc_signed&    u, long                v);
SCFP const sc_signed   operator^(const sc_signed&    u, int64               v);
SCFP const sc_signed   operator^(const sc_signed&    u, const sc_uint_base& v);
SCFP const sc_signed   operator^(const sc_signed&    u, unsigned int        v);
SCFP const sc_signed   operator^(const sc_signed&    u, uint64              v);
SCFP const sc_signed   operator^(const sc_signed&    u, unsigned long       v);

SCFP const sc_signed   operator^(const sc_signed&    u, const sc_unsigned&  v);
SCFP const sc_signed   operator^(const sc_int_base&  u, const sc_unsigned&  v);
SCFP const sc_signed   operator^(int                 u, const sc_unsigned&  v);
SCFP const sc_signed   operator^(long                u, const sc_unsigned&  v);
SCFP const sc_signed   operator^(int64               u, const sc_unsigned&  v);

SCFP const sc_signed   operator^(const sc_unsigned&  u, const sc_int_base&  v);
SCFP const sc_signed   operator^(const sc_unsigned&  u, int                 v);
SCFP const sc_signed   operator^(const sc_unsigned&  u, long                v);
SCFP const sc_signed   operator^(const sc_unsigned&  u, int64               v);

// operator == with an sc_signed argument:

SCFP bool operator==(const sc_signed&    u, const sc_signed&    v);
SCFP bool operator==(const sc_unsigned&  u, const sc_signed&    v);
SCFP bool operator==(const sc_int_base&  u, const sc_signed&    v);
SCFP bool operator==(int                 u, const sc_signed&    v);
SCFP bool operator==(long                u, const sc_signed&    v);
SCFP bool operator==(int64               u, const sc_signed&    v);
SCFP bool operator==(const sc_uint_base& u, const sc_signed&    v);
SCFP bool operator==(unsigned int        u, const sc_signed&    v);
SCFP bool operator==(uint64              u, const sc_signed&    v);
SCFP bool operator==(unsigned long       u, const sc_signed&    v);

SCFP bool operator==(const sc_signed&    u, const sc_int_base&  v);
SCFP bool operator==(const sc_signed&    u, int                 v);
SCFP bool operator==(const sc_signed&    u, long                v);
SCFP bool operator==(const sc_signed&    u, int64               v);
SCFP bool operator==(const sc_signed&    u, const sc_uint_base& v);
SCFP bool operator==(const sc_signed&    u, unsigned int        v);
SCFP bool operator==(const sc_signed&    u, uint64              v);
SCFP bool operator==(const sc_signed&    u, unsigned long       v);

SCFP bool operator==(const sc_signed&    u, const sc_unsigned&  v);
SCFP bool operator==(const sc_int_base&  u, const sc_unsigned&  v);
SCFP bool operator==(int                 u, const sc_unsigned&  v);
SCFP bool operator==(long                u, const sc_unsigned&  v);
SCFP bool operator==(int64               u, const sc_unsigned&  v);

SCFP bool operator==(const sc_unsigned&  u, const sc_int_base&  v);
SCFP bool operator==(const sc_unsigned&  u, int                 v);
SCFP bool operator==(const sc_unsigned&  u, long                v);
SCFP bool operator==(const sc_unsigned&  u, int64               v);

// operator != with an sc_signed argument:

SCFP bool operator!=(const sc_signed&    u, const sc_signed&    v);
SCFP bool operator!=(const sc_unsigned&  u, const sc_signed&    v);
SCFP bool operator!=(const sc_int_base&  u, const sc_signed&    v);
SCFP bool operator!=(int                 u, const sc_signed&    v);
SCFP bool operator!=(long                u, const sc_signed&    v);
SCFP bool operator!=(int64               u, const sc_signed&    v);
SCFP bool operator!=(const sc_uint_base& u, const sc_signed&    v);
SCFP bool operator!=(unsigned int        u, const sc_signed&    v);
SCFP bool operator!=(uint64              u, const sc_signed&    v);
SCFP bool operator!=(unsigned long       u, const sc_signed&    v);

SCFP bool operator!=(const sc_signed&    u, const sc_int_base&  v);
SCFP bool operator!=(const sc_signed&    u, int                 v);
SCFP bool operator!=(const sc_signed&    u, long                v);
SCFP bool operator!=(const sc_signed&    u, int64               v);
SCFP bool operator!=(const sc_signed&    u, const sc_uint_base& v);
SCFP bool operator!=(const sc_signed&    u, unsigned int        v);
SCFP bool operator!=(const sc_signed&    u, uint64              v);
SCFP bool operator!=(const sc_signed&    u, unsigned long       v);

SCFP bool operator!=(const sc_signed&    u, const sc_unsigned&  v);
SCFP bool operator!=(const sc_int_base&  u, const sc_unsigned&  v);
SCFP bool operator!=(int                 u, const sc_unsigned&  v);
SCFP bool operator!=(long                u, const sc_unsigned&  v);
SCFP bool operator!=(int64               u, const sc_unsigned&  v);

SCFP bool operator!=(const sc_unsigned&  u, const sc_int_base&  v);
SCFP bool operator!=(const sc_unsigned&  u, int                 v);
SCFP bool operator!=(const sc_unsigned&  u, long                v);
SCFP bool operator!=(const sc_unsigned&  u, int64               v);

// operator < with an sc_signed argument:

SCFP bool operator<(const sc_signed&    u, const sc_signed&    v);
SCFP bool operator<(const sc_unsigned&  u, const sc_signed&    v);
SCFP bool operator<(const sc_int_base&  u, const sc_signed&    v);
SCFP bool operator<(int                 u, const sc_signed&    v);
SCFP bool operator<(long                u, const sc_signed&    v);
SCFP bool operator<(int64               u, const sc_signed&    v);
SCFP bool operator<(const sc_uint_base& u, const sc_signed&    v);
SCFP bool operator<(unsigned int        u, const sc_signed&    v);
SCFP bool operator<(uint64              u, const sc_signed&    v);
SCFP bool operator<(unsigned long       u, const sc_signed&    v);

SCFP bool operator<(const sc_signed&    u, const sc_int_base&  v);
SCFP bool operator<(const sc_signed&    u, int                 v);
SCFP bool operator<(const sc_signed&    u, long                v);
SCFP bool operator<(const sc_signed&    u, int64               v);
SCFP bool operator<(const sc_signed&    u, const sc_uint_base& v);
SCFP bool operator<(const sc_signed&    u, unsigned int        v);
SCFP bool operator<(const sc_signed&    u, uint64              v);
SCFP bool operator<(const sc_signed&    u, unsigned long       v);

SCFP bool operator<(const sc_signed&    u, const sc_unsigned&  v);
SCFP bool operator<(const sc_int_base&  u, const sc_unsigned&  v);
SCFP bool operator<(int                 u, const sc_unsigned&  v);
SCFP bool operator<(long                u, const sc_unsigned&  v);
SCFP bool operator<(int64               u, const sc_unsigned&  v);

SCFP bool operator<(const sc_unsigned&  u, const sc_int_base&  v);
SCFP bool operator<(const sc_unsigned&  u, int                 v);
SCFP bool operator<(const sc_unsigned&  u, long                v);
SCFP bool operator<(const sc_unsigned&  u, int64               v);

// operator <= with an sc_signed argument:

SCFP bool operator<=(const sc_signed&    u, const sc_signed&    v);
SCFP bool operator<=(const sc_unsigned&  u, const sc_signed&    v);
SCFP bool operator<=(const sc_int_base&  u, const sc_signed&    v);
SCFP bool operator<=(int                 u, const sc_signed&    v);
SCFP bool operator<=(long                u, const sc_signed&    v);
SCFP bool operator<=(int64               u, const sc_signed&    v);
SCFP bool operator<=(const sc_uint_base& u, const sc_signed&    v);
SCFP bool operator<=(unsigned int        u, const sc_signed&    v);
SCFP bool operator<=(uint64              u, const sc_signed&    v);
SCFP bool operator<=(unsigned long       u, const sc_signed&    v);

SCFP bool operator<=(const sc_signed&    u, const sc_int_base&  v);
SCFP bool operator<=(const sc_signed&    u, int                 v);
SCFP bool operator<=(const sc_signed&    u, long                v);
SCFP bool operator<=(const sc_signed&    u, int64               v);
SCFP bool operator<=(const sc_signed&    u, const sc_uint_base& v);
SCFP bool operator<=(const sc_signed&    u, unsigned int        v);
SCFP bool operator<=(const sc_signed&    u, uint64              v);
SCFP bool operator<=(const sc_signed&    u, unsigned long       v);

SCFP bool operator<=(const sc_signed&    u, const sc_unsigned&  v);
SCFP bool operator<=(const sc_int_base&  u, const sc_unsigned&  v);
SCFP bool operator<=(int                 u, const sc_unsigned&  v);
SCFP bool operator<=(long                u, const sc_unsigned&  v);
SCFP bool operator<=(int64               u, const sc_unsigned&  v);

SCFP bool operator<=(const sc_unsigned&  u, const sc_int_base&  v);
SCFP bool operator<=(const sc_unsigned&  u, int                 v);
SCFP bool operator<=(const sc_unsigned&  u, long                v);
SCFP bool operator<=(const sc_unsigned&  u, int64               v);

// operator > with an sc_signed argument:

SCFP bool operator>(const sc_signed&    u, const sc_signed&    v);
SCFP bool operator>(const sc_unsigned&  u, const sc_signed&    v);
SCFP bool operator>(const sc_int_base&  u, const sc_signed&    v);
SCFP bool operator>(int                 u, const sc_signed&    v);
SCFP bool operator>(long                u, const sc_signed&    v);
SCFP bool operator>(int64               u, const sc_signed&    v);
SCFP bool operator>(const sc_uint_base& u, const sc_signed&    v);
SCFP bool operator>(unsigned int        u, const sc_signed&    v);
SCFP bool operator>(uint64              u, const sc_signed&    v);
SCFP bool operator>(unsigned long       u, const sc_signed&    v);

SCFP bool operator>(const sc_signed&    u, const sc_int_base&  v);
SCFP bool operator>(const sc_signed&    u, int                 v);
SCFP bool operator>(const sc_signed&    u, long                v);
SCFP bool operator>(const sc_signed&    u, int64               v);
SCFP bool operator>(const sc_signed&    u, const sc_uint_base& v);
SCFP bool operator>(const sc_signed&    u, unsigned int        v);
SCFP bool operator>(const sc_signed&    u, uint64              v);
SCFP bool operator>(const sc_signed&    u, unsigned long       v);

SCFP bool operator>(const sc_signed&    u, const sc_unsigned&  v);
SCFP bool operator>(const sc_int_base&  u, const sc_unsigned&  v);
SCFP bool operator>(int                 u, const sc_unsigned&  v);
SCFP bool operator>(long                u, const sc_unsigned&  v);
SCFP bool operator>(int64               u, const sc_unsigned&  v);

SCFP bool operator>(const sc_unsigned&  u, const sc_int_base&  v);
SCFP bool operator>(const sc_unsigned&  u, int                 v);
SCFP bool operator>(const sc_unsigned&  u, long                v);
SCFP bool operator>(const sc_unsigned&  u, int64               v);

// operator >= with an sc_signed argument:

SCFP bool operator>=(const sc_signed&    u, const sc_signed&    v);
SCFP bool operator>=(const sc_unsigned&  u, const sc_signed&    v);
SCFP bool operator>=(const sc_int_base&  u, const sc_signed&    v);
SCFP bool operator>=(int                 u, const sc_signed&    v);
SCFP bool operator>=(long                u, const sc_signed&    v);
SCFP bool operator>=(int64               u, const sc_signed&    v);
SCFP bool operator>=(const sc_uint_base& u, const sc_signed&    v);
SCFP bool operator>=(unsigned int        u, const sc_signed&    v);
SCFP bool operator>=(uint64              u, const sc_signed&    v);
SCFP bool operator>=(unsigned long       u, const sc_signed&    v);

SCFP bool operator>=(const sc_signed&    u, const sc_int_base&  v);
SCFP bool operator>=(const sc_signed&    u, int                 v);
SCFP bool operator>=(const sc_signed&    u, long                v);
SCFP bool operator>=(const sc_signed&    u, int64               v);
SCFP bool operator>=(const sc_signed&    u, const sc_uint_base& v);
SCFP bool operator>=(const sc_signed&    u, unsigned int        v);
SCFP bool operator>=(const sc_signed&    u, uint64              v);
SCFP bool operator>=(const sc_signed&    u, unsigned long       v);

SCFP bool operator>=(const sc_signed&    u, const sc_unsigned&  v);
SCFP bool operator>=(const sc_int_base&  u, const sc_unsigned&  v);
SCFP bool operator>=(int                 u, const sc_unsigned&  v);
SCFP bool operator>=(long                u, const sc_unsigned&  v);
SCFP bool operator>=(int64               u, const sc_unsigned&  v);

SCFP bool operator>=(const sc_unsigned&  u, const sc_int_base&  v);
SCFP bool operator>=(const sc_unsigned&  u, int                 v);
SCFP bool operator>=(const sc_unsigned&  u, long                v);
SCFP bool operator>=(const sc_unsigned&  u, int64               v);
