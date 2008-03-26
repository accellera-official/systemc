/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  sc_list.h -- Simple implementation of a doubly linked list.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_LIST_H
#define SC_LIST_H

namespace sc_core {

//Some forward declarations
class sc_plist_elem;
template<class T> class sc_plist_iter; 

typedef void (*sc_plist_map_fn)( void* data, void* arg );

class sc_plist_base {
    friend class sc_plist_base_iter;

public:
    sc_plist_base();
    ~sc_plist_base();
    
    typedef sc_plist_elem* handle_t;

    handle_t push_back(void* d);
    handle_t push_front(void* d);
    void* pop_back();
    void* pop_front();
    handle_t insert_before(handle_t h, void* d);
    handle_t insert_after(handle_t h, void* d);
    void* remove(handle_t h);
    void* get(handle_t h) const;
    void set(handle_t h, void* d);
    void mapcar( sc_plist_map_fn f, void* arg );

    void* front() const;
    void* back() const;

    void erase_all();
    bool empty() const { return (head == 0); }
    int size() const;

private:
    handle_t head;
    handle_t tail;
};


class sc_plist_base_iter {
public:
    typedef sc_plist_elem* handle_t;
    
    sc_plist_base_iter( sc_plist_base* l, bool from_tail = false );
    ~sc_plist_base_iter();

    void reset( sc_plist_base* l, bool from_tail = false );
    bool empty() const;
    void operator++(int);
    void operator--(int);
    void* get() const;
    void  set(void* d);
    void remove();
    void remove(int direction);

    void set_handle(handle_t h);
    handle_t get_handle() const { return ptr; }
    
private:
    sc_plist_base* lst;
    sc_plist_elem* ptr;
};

/*---------------------------------------------------------------------------*/

template< class T >
class sc_plist : public sc_plist_base {
    friend class sc_plist_iter <T>;

public:
    typedef sc_plist_iter<T> iterator;

    sc_plist() { } 
    ~sc_plist() { } 

    handle_t push_back(T d)  { return sc_plist_base::push_back((void*)d);  }
    handle_t push_front(T d) { return sc_plist_base::push_front((void*)d); }
    T pop_back()           { return (T) sc_plist_base::pop_back(); }
    T pop_front()          { return (T) sc_plist_base::pop_front(); }
    handle_t insert_before(handle_t h, T d) 
    {
        return sc_plist_base::insert_before(h, (void*) d);
    }
    handle_t insert_after(handle_t h, T d)
    {
        return sc_plist_base::insert_after(h, (void*) d);
    }
    T remove(handle_t h)
    {
        return (T)sc_plist_base::remove(h);
    }
    T get(handle_t h) const { return (T)sc_plist_base::get(h); }
    void set(handle_t h, T d) { sc_plist_base::set(h, (void*)d); }

    T front() const { return (T)sc_plist_base::front(); }
    T back() const { return (T)sc_plist_base::back(); }
};

template< class T >
class sc_plist_iter : public sc_plist_base_iter {
public:
    sc_plist_iter( sc_plist<T>* l, bool from_tail = false )
        : sc_plist_base_iter( l, from_tail )
    {

    }
    sc_plist_iter( sc_plist<T>& l, bool from_tail = false )
        : sc_plist_base_iter( &l, from_tail )
    {

    }
    ~sc_plist_iter()
    {

    }

    void reset( sc_plist<T>* l, bool from_tail = false )
    {
        sc_plist_base_iter::reset( l, from_tail );
    }
    void reset( sc_plist<T>& l, bool from_tail = false )
    {
        sc_plist_base_iter::reset( &l, from_tail );
    }

    T operator*() const { return (T) sc_plist_base_iter::get(); }
    T get() const     { return (T) sc_plist_base_iter::get(); }
    void set(T d)     { sc_plist_base_iter::set((void*) d); }
};

} // namespace sc_core

#endif
