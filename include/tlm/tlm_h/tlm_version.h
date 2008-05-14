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
 
/* ---------------------------------------------------------------------------------------
 @file tlm_version.h
 
 @brief TLM version header
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __TLM_VERSION_H__
#define __TLM_VERSION_H__

#define TLM_VERSION_MAJOR                   2
#define TLM_VERSION_MINOR                   0
#define TLM_VERSION_PATCH                   0
#define TLM_VERSION_SEPARATOR               "."

#define TLM_IS_PRERELEASE                   TRUE
#define TLM_VERSION_PRERELEASE              "DR3"

/************************** do not modify below this line *******************************/

#define TLM_VERSION_STR(x)                  TLM_VERSION_STR_HELPER(x)
#define TLM_VERSION_STR_HELPER(x)           #x

#define TLM_VERSION_STRING_MAJOR            TLM_VERSION_STR(TLM_VERSION_MAJOR)
#define TLM_VERSION_STRING_MINOR            TLM_VERSION_STR(TLM_VERSION_MINOR)
#define TLM_VERSION_STRING_PATCH            TLM_VERSION_STR(TLM_VERSION_PATCH)

#define TLM_VERSION_STRING_MMP              TLM_VERSION_STRING_MAJOR TLM_VERSION_SEPARATOR \
                                            TLM_VERSION_STRING_MINOR TLM_VERSION_SEPARATOR \
                                            TLM_VERSION_STRING_PATCH
                                    
#if ( TLM_IS_PRERELEASE == TRUE )

#define TLM_VERSION_STRING_PRE_START        " ("
#define TLM_VERSION_STRING_PRERELEASE       TLM_VERSION_PRERELEASE
#define TLM_VERSION_STRING_PRE_END          ")"

#else   /* TLM_IS_PRERELEASE == TRUE */

#define TLM_VERSION_STRING_PRE_START        ""
#define TLM_VERSION_STRING_PRERELEASE       ""
#define TLM_VERSION_STRING_PRE_END          ""

#endif  /* TLM_IS_PRERELEASE == TRUE */

/****************************** use these within code ***********************************/

#define TLM_VERSION_STRING                  TLM_VERSION_STRING_MMP \
                                            TLM_VERSION_STRING_PRE_START \
                                            TLM_VERSION_STRING_PRERELEASE \
                                            TLM_VERSION_STRING_PRE_END
                                        
#define TLM_VERSION                         TLM_VERSION_STRING

const unsigned int  tlm_version_major       ( TLM_VERSION_MAJOR         );
const unsigned int  tlm_version_minor       ( TLM_VERSION_MINOR         );
const unsigned int  tlm_version_patch       ( TLM_VERSION_PATCH         );

const bool          tlm_is_prerelease       (
#if ( TLM_IS_PRERELEASE == TRUE )
                                              true
#else   /* TLM_IS_PRERELEASE == TRUE */
                                              false
#endif  /* TLM_IS_PRERELEASE == TRUE */
                                            );

const std::string   tlm_version_string      ( TLM_VERSION_STRING        );
const std::string   tlm_version_prerelease  ( TLM_VERSION_PRERELEASE    );

#endif /* __TLM_VERSION_H__ */
