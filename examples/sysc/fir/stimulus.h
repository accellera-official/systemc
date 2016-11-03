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

/*****************************************************************************
 
  stimulus.h -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

SC_MODULE(stimulus) {

  sc_out<bool> reset;
  sc_out<bool> input_valid;   
  sc_out<int>  sample;  	    
  sc_in<bool>  CLK;

  sc_int<8>     send_value1;
  unsigned cycle;

  SC_CTOR(stimulus)
  {
      SC_METHOD(entry);
      dont_initialize();
      sensitive << CLK.pos();
      send_value1 = 0;
      cycle       = 0;
  }  
  void entry();
};

