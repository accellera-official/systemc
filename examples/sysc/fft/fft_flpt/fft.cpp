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


#include "systemc.h"
#include "fft.h"
#include "math.h"

void fft::entry()
{ float sample[16][2];
  unsigned int index;

  while(true)
  { data_req.write(false);
    data_ready.write(false);
    index = 0;
    //Reading in the Samples
      cout << endl << "Reading in the samples..." << endl;
      while( index < 16 )
      {
       data_req.write(true);
       do { wait(); } while (data_valid == true);
       sample[index][0] = in_real.read();
       sample[index][1] = in_imag.read();
       index++;
       data_req.write(false);
       wait();
      }
      index = 0;


      //////////////////////////////////////////////////////////////////////////
      ///  Computation - 1D Complex DFT In-Place DIF Computation Algorithm  ////
      //////////////////////////////////////////////////////////////////////////
    
      //Size of FFT, N = 2**M    
       unsigned int N, M, len ;  
       float theta; 
       float W[7][2], w_real, w_imag, w_rec_real, w_rec_imag, w_temp;

       //Initialize
       M = 4; N = 16; 
       len = N/2;
       theta = 8.0*atan(1.0)/N;

       cout << "Computing..." << endl;

       //Calculate the W-values recursively
        w_real =  cos(theta);
        w_imag =  -sin(theta);

        w_rec_real = 1;
        w_rec_imag = 0;

        index = 0;    
        while(index < len-1) 
        {
           w_temp = w_rec_real*w_real - w_rec_imag*w_imag;
           w_rec_imag =  w_rec_real*w_imag + w_rec_imag*w_real;
           w_rec_real = w_temp;
           W[index][0] = w_rec_real;
           W[index][1] = w_rec_imag;
           index++;
        }


       float tmp_real, tmp_imag, tmp_real2, tmp_imag2;
       unsigned int stage, i, j,index2, windex, incr;
     
      //Begin Computation 
       stage = 0;

       len = N;
       incr = 1;

       while (stage < M)
       { 
        len = len/2;
 
        //First Iteration :  With No Multiplies
          i = 0;
 

          while(i < N)
          {
             index =  i; index2 = index + len; 
             
             tmp_real = sample[index][0] + sample[index2][0];
             tmp_imag = sample[index][1] + sample[index2][1];
 
             sample[index2][0] = sample[index][0] - sample[index2][0];
             sample[index2][1] = sample[index][1] - sample[index2][1];

             sample[index][0] = tmp_real;
             sample[index][1] = tmp_imag;
        

             i = i + 2*len;          
 
          }


        //Remaining Iterations: Use Stored W
         j = 1; windex = incr - 1;
         while (j < len) // This loop executes N/2 times at first stage, .. once at last stage.
         {
            i = j; 
            while (i < N)
            {
              index = i;
              index2 = index + len;

              tmp_real = sample[index][0] + sample[index2][0]; 
              tmp_imag = sample[index][1] + sample[index2][1];
              tmp_real2 = sample[index][0] - sample[index2][0];
              tmp_imag2 = sample[index][1] - sample[index2][1];
  
              sample[index2][0] = tmp_real2*W[windex][0] - tmp_imag2*W[windex][1];
              sample[index2][1] = tmp_real2*W[windex][1] + tmp_imag2*W[windex][0]; 

              sample[index][0] = tmp_real;
              sample[index][1] = tmp_imag;
           
              i = i + 2*len;
              
            }
            windex = windex + incr;
            j++;
         }
          stage++;
          incr = 2*incr;
       } 
           
     //////////////////////////////////////////////////////////////////////////
     
     //Writing out the normalized transform values in bit reversed order
      sc_uint<4> bits_i;
      sc_uint<4> bits_index;
      bits_i = 0;
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
       out_real.write(sample[index][0]);
       out_imag.write(sample[index][1]);
       data_ready.write(true);
       do { wait(); } while ( !(data_ack == true) );
       data_ready.write(false);
       i++;
       wait();
      }
      index = 0; 
      cout << "Done..." << endl;
  }
 }
