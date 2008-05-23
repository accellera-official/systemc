/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/
/*
 * @file
 *  reporting.h
 *  
 * @brief
 * Convenience macro to simplify reporting.
 */
#ifndef __REPORTING_H__
#define __REPORTING_H__

#include <iomanip>
#include <iostream>

#define REPORT_HEAD "\t"<<name()<<"\t\t"
#define HEX_ADDR std::internal<<std::setw(8)<<std::setfill('0')<<std::uppercase<<std::hex
#define HEX_DATA std::internal<<std::setw(2)<<std::setfill('0')<<std::uppercase<<std::hex

#endif /* __REPORTING_H__ */
