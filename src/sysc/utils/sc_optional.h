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

  sc_optional.h - Container for conditionally created
                  sc_object ( module, port, channel )

  sc_optional<T> is a better replacement for T * opt = new T("opt") :
  - Explicitly specifies designer intent
  - Supports initialization outside of host module constructor
  - Does not use dynamic allocation, thus can be faster

  Implementation is similar to std::optional, but is much simpler since it
  has to deal only with sc_objects

 *****************************************************************************/

#ifndef SC_OPTIONAL_H_INCLUDED_
#define SC_OPTIONAL_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h"

#if SC_CPLUSPLUS >= 201103L

#include "sysc/kernel/sc_object.h"
#include "sysc/utils/sc_ptr_flag.h"
#include "sysc/utils/sc_report.h"
#include <type_traits>
#include <cstdint>

namespace sc_core {

SC_API_TEMPLATE_DECL_ sc_ptr_flag<sc_object>;

class SC_API sc_optional_base
{
public:
    sc_optional_base();
    bool has_value() const;
    explicit operator bool() const;

protected:
    sc_hierarchy_scope restore_hierarchy();
    void set_has_value();
    void report_double_init(const char* existing_obj_name) const;
private:
    // has_value flag is stored in LSB of m_parent_with_flag
    sc_ptr_flag<sc_object> m_parent_with_flag;
};

inline bool sc_optional_base::has_value() const
{
    return m_parent_with_flag.get_flag();
}

inline sc_optional_base::operator bool() const
{
    return has_value();
}

template <typename T>
class sc_optional : public sc_optional_base {
public:
    static_assert( std::is_base_of<sc_object, T>::value
                   || std::is_base_of<sc_event, T>::value ,
        "sc_optional shall only be used with sc_objects and sc_events");

    typedef T value_type;

    sc_optional() {}

    template <typename... Args>
    explicit sc_optional(Args... args);

    template <typename... Args>
    void init(Args ...);

    ~sc_optional();

    T* operator->();
    const T* operator->() const;

    T& operator *();
    const T& operator *() const;

private:
    union {
        T m_val;
    };
};

template <typename T>
template <typename... Args>
sc_optional<T>::sc_optional(Args... args)
    : m_val(std::forward<Args>(args)...)
{
    set_has_value();
}

template<typename T>
template<typename... Args>
void sc_optional<T>::init(Args... args)
{
    if (!has_value()) {
        sc_hierarchy_scope scope{ restore_hierarchy() };
        new (&m_val) T(std::forward<Args>(args)...);
        set_has_value();
    } else {
        report_double_init( (*this)->name() );
    }
}

template <typename T>
sc_optional<T>::~sc_optional()
{
    if (has_value())
        m_val.~T();
}

template<typename T>
inline T *sc_optional<T>::operator->()
{
    sc_assert(has_value());
    return &m_val;
}

template<typename T>
inline const T *sc_optional<T>::operator->() const
{
    sc_assert(has_value());
    return &m_val;
}

template<typename T>
inline T &sc_optional<T>::operator*()
{
    sc_assert(has_value());
    return m_val;
}

template<typename T>
inline const T &sc_optional<T>::operator*() const
{
    sc_assert(has_value());
    return m_val;
}

} // namespace sc_core

#endif // SC_CPLUSPLUS >= 201103L
#endif // SC_OPTIONAL_H_INCLUDED_
