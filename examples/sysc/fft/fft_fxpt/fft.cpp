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

  fft.cpp - This is the implementation file for the synchronous process "fft".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


/* This is the implementation file for the synchronous process "fft" */
#include "systemc.h"
#include "fft.h"


//Function for butterfly computation

 void func_butterfly
    ( const sc_int<16>& w_real   /* snps width 16 */,
      const sc_int<16>& w_imag   /* snps width 16 */, 
      const sc_int<16>& real1_in /* snps width 16 */,
      const sc_int<16>& imag1_in /* snps width 16 */,
      const sc_int<16>& real2_in /* snps width 16 */,
      const sc_int<16>& imag2_in /* snps width 16 */,
      sc_int<16>& real1_out /* snps width 16 */,
      sc_int<16>& imag1_out /* snps width 16 */,
      sc_int<16>& real2_out /* snps width 16 */,
      sc_int<16>& imag2_out /* snps width 16 */
    )
 {

   // Variable declarations
     sc_int<17> tmp_real1;
     sc_int<17> tmp_imag1;
     sc_int<17> tmp_real2;
     sc_int<17> tmp_imag2;
     sc_int<34> tmp_real3;
     sc_int<34> tmp_imag3;
  

    // Begin Computation

    tmp_real1 = real1_in + real2_in; 

    // <s,6,10> = <s,5,10> + <s,5,10>
    tmp_imag1 = imag1_in + imag2_in;

    // <s,6,10> = <s,5,10> - <s,5,10>
    tmp_real2 = real1_in - real2_in;

    // <s,6,10> = <s,5,10> - <s,5,10>
    tmp_imag2 = imag1_in - imag2_in;

    //   <s,13,20> = <s,6,10>*<s,5,10> - <s,6,10>*<s,5,10>
    tmp_real3 = tmp_real2*w_real - tmp_imag2*w_imag;

    //   <s,13,20> = <s,6,10>*<s,5,10> - <s,6,10>*<s,5,10>
    tmp_imag3 = tmp_real2*w_imag + tmp_imag2*w_real; 

    // assign the sign-bit(MSB)      
    real1_out[15] = tmp_real1[16];
    imag1_out[15] = tmp_imag1[16];

    // assign the rest of the bits
    real1_out.range(14,0) = tmp_real1.range(14,0);
    imag1_out.range(14,0) = tmp_imag1.range(14,0);

   // assign the sign-bit(MSB)      
    real2_out[15] = tmp_real3[33];
    imag2_out[15] = tmp_imag3[33];          

   // assign the rest of the bits
    real2_out.range(14,0) = tmp_real3.range(24,10);
    imag2_out.range(14,0) = tmp_imag3.range(24,10);

 }; // end func_butterfly

void fft::entry()
{ 
 // Variable Declarations
  sc_int<16> real[16];
  sc_int<16> imag[16];
  sc_int<16> tmp_real;
  sc_int<16> tmp_imag;
  short index;
  sc_int<6> N;  
  sc_int<4> M;
  sc_int<6> len;
  sc_int<16> W_real[7];
  sc_int<16> W_imag[7];
  sc_int<16> w_real;
  sc_int<16> w_imag;
  sc_int<16> w_rec_real;
  sc_int<16> w_rec_imag;
  sc_int<32> w_temp1;
  sc_int<32> w_temp2;
  sc_int<32> w_temp3;
  sc_int<32> w_temp4;
  sc_int<33> w_temp5;
  sc_int<33> w_temp6;
  sc_int<16> real1_in;
  sc_int<16> imag1_in;
  sc_int<16> real2_in;
  sc_int<16> imag2_in;
  sc_int<16> real1_out;
  sc_int<16> imag1_out;
  sc_int<16> real2_out;
  sc_int<16> imag2_out;
  sc_int<4> stage;
  short i;
  short j;
  short index2;
  short windex;
  short incr;

  while(true)
  { data_req.write(false);
    data_ready.write(false);
    index = 0; 
     
    wait();
    //Read in the Sample values
      cout << endl << "Reading in the samples..." << endl;
      while( index < 16 )
      {
       data_req.write(true);
       do { wait(); } while ( !(data_valid == true) );
       tmp_real = in_real.read();
       tmp_imag = in_imag.read();
       real[index] = tmp_real;
       imag[index] = tmp_imag;
       index++;
       data_req.write(false);
       wait();
      }
      index = 0;

 
       // Initialize
       M = 4; N = 16; 
       len = N >> 1;

       cout << "Computing..." << endl;
       // Calculate the W-values recursively
       // <'s'/'u',m,n>: is used in comments to denote a fixed point representation
       // 's'- signed, 'u'- unsigned, m - no. of integer bits, n - no. of fractional bits

       //  theta = 8.0*atan(1.0)/N; theta = 22.5 degree

       //  w_real =  cos(theta) = 0.92 (000000.1110101110) <s,5,10>
           w_real =  942;

       //  w_imag = -sin(theta) = -0.38(111111.1001111010) <s,5,10>
           w_imag = -389;

       //  w_rec_real = 1(0000001.0000000000)
	   w_rec_real = 1024;

       //  w_rec_real = 0(000000.0000000000)	 
           w_rec_imag = 0;

        unsigned short w_index;

        w_index = 0;    
        while(w_index < 7) 
        {
	  // <s,11,20> = <s,5,10> * <s,5,10>
	   w_temp1 = w_rec_real*w_real;
	   w_temp2 = w_rec_imag*w_imag;

          // <s,11,20> = <s,5,10> * <s,5,10>
	   w_temp3 = w_rec_real*w_imag;
	   w_temp4 = w_rec_imag*w_real;	 

	  // <s,6,10> = <s,5,10> - <s,5,10>
           w_temp5 = w_temp1 - w_temp2;

	  // <s,6,10> = <s,5,10> + <s,5,10>
	   w_temp6 = w_temp3 + w_temp4;
	   
	  // assign the sign-bit(MSB)
           W_real[w_index][15] = w_temp5[32];
           W_imag[w_index][15] = w_temp6[32];

	  // assign the rest of the bits
           W_real[w_index].range(14,0) = w_temp5.range(24,10);
           W_imag[w_index].range(14,0) = w_temp6.range(24,10);

	  // update w_rec.. values for the next iteration
	   w_rec_real = W_real[w_index];
	   w_rec_imag = W_imag[w_index];

	   w_index++;

        }

      //////////////////////////////////////////////////////////////////////////
      ///  Computation - 1D Complex DFT In-Place DIF Computation Algorithm  ////
      //////////////////////////////////////////////////////////////////////////

       stage = 0;
       len = N;
       incr = 1;

       while (stage < M) 
       { 
	 len = len >> 1;
 
        //First Iteration :  Simple calculation, with no multiplies
          i = 0;
          while(i < N)  
          {
             index =  i; index2 = i + len; 

             tmp_real = real[index] + real[index2];
             tmp_imag = imag[index] + imag[index2];

             real[index2] = (real[index] - real[index2]);
             imag[index2] = (imag[index] - imag[index2]);

             real[index] = tmp_real;
             imag[index] = tmp_imag;
	    
	     i = i + (len << 1);   
          }

        //Remaining Iterations: Use Stored W
         j = 1; windex = incr - 1;
        // This loop executes N/2 times at the first stage, N/2 times at the second.. once at last stage
         while (j < len)
         {
            i = j; 
            while (i < N) 
            {
              index = i;
              index2 = i + len;

	      // Read in the data and twiddle factors
	      w_real  = W_real[windex];
	      w_imag  = W_imag[windex];

              real1_in = real[index];
	      imag1_in = imag[index];
              real2_in = real[index2];
	      imag2_in = imag[index2];

              // Call butterfly computation function	     
	      func_butterfly(w_real, w_imag, real1_in, imag1_in, real2_in, imag2_in, real1_out, imag1_out, real2_out, imag2_out);

	      // Store back the results
              real[index]  = real1_out;
              imag[index]  = imag1_out; 
              real[index2] = real2_out;
              imag[index2] = imag2_out; 

              i = i + (len << 1);
            }
            windex = windex + incr;
            j++;
         }
          stage++;
          incr = incr << 1;
       } 
           
     //////////////////////////////////////////////////////////////////////////   
     //Writing out the normalized transform values in bit reversed order
     //////////////////////////////////////////////////////////////////////////

      sc_uint<4> bits_i;
      sc_uint<4> bits_index;
      sc_int<16> real1;
      sc_int<16> imag1;
      bits_i = 0;
      bits_index = 0;
      i = 0;

      cout << "Writing the transform values..." << endl;
      while( i < 16)
      {
       bits_i = i;
       bits_index[3]= bits_i[0];
       bits_index[2]= bits_i[1];
       bits_index[1]= bits_i[2];
       bits_index[0]= bits_i[3];
       index = bits_index;
       real1 = real[index];
       imag1 = imag[index];
       out_real.write(real1); 
       out_imag.write(imag1); 
       data_ready.write(true);
       do { wait(); } while ( !(data_ack == true) );
       data_ready.write(false);
       i++;
       wait();
      }
      index = 0; 
      cout << "Done..." << endl;
  }      
}// end entry() function
