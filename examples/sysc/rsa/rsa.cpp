/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

/*****************************************************************************
 
  rsa.cpp -- An implementation of the RSA public-key cipher. The
             following implementation is based on the one given in Cormen et
             al., Inroduction to Algorithms, 1991. I'll refer to this book as
             CLR because of its authors. This implementation shows the usage of
             arbitrary precision types of SystemC. That is, these types in
             SystemC can be used to implement algorithmic examples regarding
             arbitrary precision integers. The algorithms used are not the most
             efficient ones; however, they are intended for explanatory
             purposes, so they are simple and perform their job correctly.
             Below, NBITS shows the maximum number of bits in n, the variable
             that is a part of both the public and secret keys, P and S,
             respectively. NBITS can be made larger at the expense of longer
             running time. For example, CLR mentions that the RSA cipher uses
             large primes that contain approximately 100 decimal digits. This
             means that NBITS should be set to approximately 560.

             Some background knowledge: A prime number p > 1 is an integer that
             has only two divisiors, 1 and p itself. For example, 2, 3, 5, 7,
             and 11 are all primes. If p is not a prime number, it is called a
             composite number. If we are given two primes p and q, it is easy
             to find their product p * q; however, if we are given a number m
             which happens to be the product of two primes p and q that we do
             not know, it is very difficult to find p and q if m is very large,
             i.e., it is very difficult to factor m. The RSA public-key
             cryptosystem is based on this fact. Internally, we use the
             Miller-Rabin randomized primality test to deal with primes. More
             information can be obtained from pp. 831-836 in CLR, the first
             edition.

  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>    // drand48, srand48
#include "systemc.h"

#define DEBUG_SYSTEMC // #undef this to disable assertions.

// NBITS is the number of bits in n of public and secret keys P and
// S. HALF_NBITS is the number of bits in p and q, which are the prime
// factors of n.
#define NBITS         250
#define HALF_NBITS    ( NBITS / 2 )    

// +2 is for the format specifier '0b' to make the string binary.
#define STR_SIZE      ( NBITS + 2 ) 
#define HALF_STR_SIZE ( HALF_NBITS + 2 )

typedef sc_bigint<NBITS>  bigint;

// Return the absolute value of x.
inline 
bigint
abs_val( const sc_signed& x ) 
{
  return ( x < 0 ? -x : x );
}

// Initialize the random number generator. If seed == -1, the
// generator will be initialized with the system time. If not, it will
// be initialized with the given seed. This way, an experiment with
// random numbers becomes reproducible.
inline
long
randomize( int seed  )
{
  long in_seed;  // time_t is long.

  in_seed = ( seed <= 0 ? time( 0 ) : seed );

#ifndef WIN32
  srand48( in_seed );
#else
  srand( ( unsigned ) in_seed );
#endif

  return in_seed;
}

// Flip a coin with probability p.

#ifndef WIN32

inline
bool
flip( double p )
{
  return ( drand48() < p );
}

#else

inline
bool
flip( double p )
{
  const int MAX_VAL = ( 1 << 15 );

  // rand() produces an integer between 0 and 2^15-1, so rand() /
  // MAX_VAL is a number between 0 and 1, which is required to compare
  // with p.
  return ( rand() < ( int ) ( p * MAX_VAL ) );
}

#endif

// Randomly generate a bit string with nbits bits.  str has a length
// of nbits + 1. This function is used to generate random messages to
// process.
inline
void
rand_bitstr( char *str, int nbits )
{
  assert( nbits >= 4 );

  str[ 0 ] = '0';
  str[ 1 ] = 'b';
  str[ 2 ] = '0';  // Sign for positive numbers.

  for ( int i = 3; i < nbits; ++i )
    str[ i ] = ( flip( 0.5 ) == true ? '1' : '0' );

  str[ nbits ] = '\0';
}

// Generate "111..111" with nbits bits for masking.
// str has a length of nbits + 1.
inline
void
max_bitstr( char *str, int nbits )
{
  assert( nbits >= 4 );

  str[ 0 ] = '0';
  str[ 1 ] = 'b';  
  str[ 2 ] = '0';  // Sign for positive numbers.

  for ( int i = 3; i < nbits; ++i )
    str[ i ] = '1';

  str[ nbits ] = '\0';
}

// Return a positive remainder.
inline
bigint
ret_pos( const bigint& x, const bigint& n )
{
  if ( x < 0 )
    return x + n;
  return x;
}

// Compute the greatest common divisor ( gcd ) of a and b. This is
// Euclid's algorithm. This algorithm is at least 2,300 years old! The
// non-recursive version of this algorithm is not as elegant.
bigint
gcd( const bigint& a, const bigint& b )
{
  if ( b == 0 )
    return a;
  return gcd( b, a % b );
}

// Compute d, x, and y such that d = gcd( a, b ) = ax + by. x and y can
// be zero or negative. This algorithm is also Euclid's algorithm but
// it is extended to also find x and y. Recall that the existence of x
// and y is guaranteed by Euclid's algorithm.
void
euclid( const bigint& a, const bigint& b, bigint& d, bigint& x, bigint& y )
{
  if ( b != 0 ) {
    euclid( b, a % b, d, x, y );

    bigint tmp = x;
    x = y;
    y = tmp - ( a / b ) * y;
  }
  else {
    d = a;
    x = 1;
    y = 0;
  }
}

// Return d = a^b % n, where ^ represents exponentiation.
inline
bigint
modular_exp( const bigint& a, const bigint& b, const bigint& n )
{
  bigint d = 1;

  for ( int i = b.length() - 1; i >= 0; --i )
  {
    d = ( d * d ) % n;
    if ( b[ i ] )
      d = ( d * a ) % n;
  }

  return ret_pos( d, n );
}

// Return the multiplicative inverse of a, modulo n, when a and n are
// relatively prime. Recall that x is a multiplicative inverse of a,
// modulo n, if a * x = 1 ( mod n ). 
inline
bigint
inverse( const bigint& a, const bigint& n )
{
  bigint d, x, y;

  euclid( a, n, d, x, y );
  assert( d == 1 );
  x %= n;

  return ret_pos( x, n );
}

// Find a small odd integer a that is relatively prime to n. I do not
// know an efficient algorithm to do that but the loop below seems to
// work; it usually iterates a few times. Recall that a is relatively
// prime to n if their only common divisor is 1, i.e., gcd( a, n ) ==
// 1.
inline
bigint
find_rel_prime( const bigint& n )
{
  bigint a = 3;
  while ( true ) {
    if ( gcd( a, n ) == 1 )
      break;
    a += 2;
#ifdef DEBUG_SYSTEMC
    assert( a < n );
#endif
  }

  return a;
}

// Return true if and only if a is a witness to the compositeness of
// n, i.e., a can be used to prove that n is composite.
inline
bool
witness( const bigint& a, const bigint& n )
{
  bigint n_minus1 = n - 1;
  bigint x;
  bigint d = 1;

  // Compute d = a^( n-1 ) % n.
  for ( int i = n.length() - 1; i >= 0; --i )
  {
    // Sun's SC5 bug when compiling optimized version
    // makes the wrong assignment if abs_val() is inlined
    //x = (sc_signed)d<0?-(sc_signed)d:(sc_signed)d;//abs_val( d );
    if(d<0)
      {
      x = -d;
      assert(x==-d);
      }
    else
      {
      x = d;
      assert(x==d);
      }
    d = ( d * d ) % n;

    // x is a nontrivial square root of 1 modulo n ==> n is composite.
    if ( ( abs_val( d ) == 1 ) && ( x != 1 ) && ( x != n_minus1 ) )
      return true;  

    if ( n_minus1[ i ] )
      d = ( d * a ) % n;
  }

  // d = a^( n-1 ) % n != 1 ==> n is composite.
  if ( abs_val( d ) != 1 )
    return true;  

  return false;
}

// Check to see if n has any small divisors. For small numbers, we do
// not have to run the Miller-Rabin primality test. We define "small"
// to be less than 1023. You can change it if necessary.
inline
bool
div_test( const bigint& n )
{
  int limit;

  if ( n < 1023 )
    limit = n.to_int() - 2;
  else
    limit = 1023;

  for ( int i = 3; i <= limit; i += 2 ) {
    if ( n % i == 0 )
      return false;   // n is composite.
  }

  return true;  // n may be prime.
}

// Return true if n is almost surely prime, return false if n is
// definitely composite.  This test, called the Miller-Rabin primality
// test, errs with probaility at most 2^(-s). CLR suggests s = 50 for
// any imaginable application, and s = 3 if we are trying to find
// large primes by applying miller_rabin to randomly chosen large
// integers. Even though we are doing the latter here, we will still
// choose s = 50. The probability of failure is at most
// 0.00000000000000088817, a pretty small number.
inline
bool
miller_rabin( const bigint& n )
{
  if ( n <= 2 )
    return false;

  if ( ! div_test( n ) )
    return false;
              
  char str[ STR_SIZE + 1 ];

  int s = 50;
  for ( int j = 1; j <= s; ++j ) {

    // Choose a random number.
    rand_bitstr( str, STR_SIZE );

    // Set a to the chosen number.
    bigint a = str;

    // Make sure that a is in [ 1, n - 1 ].
    a = ( a % ( n - 1 ) ) + 1;

    // Check to see if a is a witness.
    if ( witness( a, n ) )
      return false;  // n is definitely composite. 
  }

  return true;   // n is almost surely prime. 
}

// Return a randomly generated, large prime number using the
// Miller-Rabin primality test.
inline
bigint
find_prime( const bigint& r )
{
  char p_str[ HALF_STR_SIZE + 1 ];

  rand_bitstr( p_str, HALF_STR_SIZE );
  p_str[ HALF_STR_SIZE - 1 ] = '1';  // Force p to be an odd number.

  bigint p = p_str;

#ifdef DEBUG_SYSTEMC
  assert( ( p > 0 ) && ( p % 2 == 1 ) );
#endif

  // p is randomly determined. Now, we'll look for a prime in the
  // vicinity of p. By the prime number theorem, executing the
  // following loop approximately ln ( 2^NBITS ) iterations should
  // find a prime.

#ifdef DEBUG_SYSTEMC
  // A very large counter to check against infinite loops.
  sc_bigint<NBITS> niter = 0;
#endif

  while ( ! miller_rabin( p ) ) {
    p = ( p + 2 ) % r;

#ifdef DEBUG_SYSTEMC
    assert( ++niter > 0 );
#endif
  }

  return p;
}

// Encode or cipher the message in msg using the RSA public key P=( e, n ).
inline
bigint
cipher( const bigint& msg, const bigint& e, const bigint& n )
{
  return modular_exp( msg, e, n );
}

// Dencode or decipher the message in msg using the RSA secret key S=( d, n ).
inline
bigint
decipher( const bigint& msg, const bigint& d, const bigint& n )
{
  return modular_exp( msg, d, n );
}

// The RSA cipher.
inline
void
rsa( int seed )
{
  // Generate all 1's in r.
  char r_str[ HALF_STR_SIZE + 1 ];

  max_bitstr( r_str, HALF_STR_SIZE );
  bigint r = r_str;

#ifdef DEBUG_SYSTEMC
  assert( r > 0 );
#endif

  // Initialize the random number generator.
  cout << "\nRandom number generator seed = " << randomize( seed ) << endl;

  cout << endl;

  // Find two large primes p and q.
  bigint p = find_prime( r );
  bigint q = find_prime( r );

#ifdef DEBUG_SYSTEMC
  assert( ( p > 0 ) && ( q > 0 ) );
#endif

  // Compute n and ( p - 1 ) * ( q - 1 ) = m.
  bigint n = p * q;
  bigint m = ( p - 1 ) * ( q - 1 );

#ifdef DEBUG_SYSTEMC
  assert( ( n > 0 ) && ( m > 0 ) );
#endif
  // Find a small odd integer e that is relatively prime to m.
  bigint e = find_rel_prime( m );

#ifdef DEBUG_SYSTEMC
  assert( e > 0 );
#endif

  // Find the multiplicative inverse d of e, modulo m.
  bigint d = inverse( e, m );

#ifdef DEBUG_SYSTEMC
  assert( d > 0 );
#endif

  // Output public and secret keys.
  cout << "RSA public key P: P=( e, n )" << endl;
  cout << "e = " << e << endl;
  cout << "n = " << n << endl;
  cout << endl;

  cout << "RSA secret key S: S=( d, n )" << endl;
  cout << "d = " << d << endl;
  cout << "n = " << n << endl;
  cout << endl;

  // Cipher and decipher a randomly generated message msg.
  char msg_str[ STR_SIZE + 1 ];
  rand_bitstr( msg_str, STR_SIZE );
  bigint msg = msg_str;

  msg %= n; // Make sure msg is smaller than n. If larger, this part
            // will be a block of the input message.

#ifdef DEBUG_SYSTEMC
  assert( msg > 0 );
#endif

  cout << "Message to be ciphered = " << endl;
  cout << msg << endl;

  bigint msg2 = cipher( msg, e, n );
  cout << "\nCiphered message = " << endl;
  cout << msg2 << endl;

  msg2 = decipher( msg2, d, n );
  cout << "\nDeciphered message = " << endl;
  cout << msg2 << endl;

  // Make sure that the original message is recovered.
  if ( msg == msg2 ) {
    cout << "\nNote that the original message == the deciphered message, " << endl;
    cout << "showing that this algorithm and implementation work correctly.\n" << endl;
  }
  else {
    // This case is unlikely.
    cout << "\nNote that the original message != the deciphered message, " << endl;
    cout << "showing that this implementation works incorrectly.\n" << endl;
  }

  return;
}

int sc_main( int argc, char *argv[] )
{
  if ( argc <= 1 )
    rsa( -1 );
  else
    rsa( atoi( argv[ 1 ] ) );

  return 0;
}

// End of file
