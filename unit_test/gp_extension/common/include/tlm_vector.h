/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

  Author: Olaf Scheufen
*****************************************************************************/

#ifndef TLM_VECTOR_H
#define TLM_VECTOR_H

#include <exception>
#include <assert.h>

#include <systemc>

namespace tlm {

// This implements a fast vector class with limited functionality
// compared to std::vector. The class is primarily used in the
// tlm_generic_payload class for storing the pointers to the extensions.
//
// Individual vector elements can be accessed through the [] operators, or
// by accessing the m_data array directly.
//
// The size can be dynamically expanded using the expand(uint) method. There
// is no shrinking mechanism implemented, because the extension mechanism
// -as primary use case- does not require this feature.

template <typename T>
class tlm_vector
{
public:

    // constructor:
    tlm_vector(unsigned int size = 0, T default_value = 0)
        : m_data(0)
        , m_size(0)
        , m_default(default_value)
    {
        expand(size);
    }

    // copy constructor:
    tlm_vector(const tlm_vector& orig)
    {
        m_size = orig.size();
        if (m_size>0)
        {
            try {
                m_data = new T[m_size];
            }
            catch (std::exception &e) {
                report_error(e); return;
            }
            for(unsigned int i=0; i<m_size; i++)
            {
                m_data[i] = orig.m_data[i];
            }
        }
    }

    // destructor:
    ~tlm_vector()
    {
        if (m_size>0) delete[] m_data;
        m_size = 0;
    }

    // operators for dereferencing:
    T& operator[](const unsigned int index)
    {
        // TODO: should we enable assertions here? If the user doesn't define
        // NDEBUG manually, the assert would cost performance:
        // assert(index < m_size);
        return m_data[index];
    }
    const T& operator[](const unsigned int index) const
    {
        // assert(index < m_size);
        return m_data[index];
    }

    // vector size:
    unsigned int size() const {return m_size;}

    // expand the vector if needed:
    void expand(unsigned int new_size)
    {
        if (new_size > m_size)
        {
            unsigned int i;
            if(m_size==0)
            {
                try {
                    m_data = new T[new_size];
                }
                catch (std::exception &e) {
                    report_error(e); return;
                }
                for(i=0; i<new_size; i++)
                {
                    m_data[i] = m_default;
                }
                m_size = new_size;
            }
            else
            {
                T* tmp = m_data;
                try {
                    m_data = new T[new_size];
                }
                catch (std::exception &e) {
                    report_error(e); return;
                }
                for(i=0; i<m_size; i++)
                {
                    m_data[i] = tmp[i];
                }
                for(i=0; i<new_size; i++)
                {
                    m_data[i] = m_default;
                }
                m_size = new_size;
                delete[] tmp;
            }
        }
    }

    static const char* const kind_string;
    const char* kind() const
        { return kind_string; }

    // direct access to the data vector is allowed:
    T* m_data;

private:
    unsigned int m_size;
    T m_default;

    // disabled:
    tlm_vector& operator=(const tlm_vector<T>&);
    
    // Report allocation error:
    void report_error(std::exception &e)
    {
        std::string msg("Allocation of vector failed: ");
        msg += e.what();
        SC_REPORT_FATAL(kind_string, msg.c_str());
    }
};


template <typename T>
const char* const tlm_vector<T>::kind_string = "tlm_vector";

} // namespace tlm

#endif
