/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#ifndef __EXTENSION_POOL_H__
#define __EXTENSION_POOL_H__

template <class T>
class ExtensionPool{
  struct entry{
    public:
    entry(T* content){
      that=content;
      next=NULL;
    }
    T* that;
    entry* next;
  };
  
public:
  ExtensionPool(int size): used(NULL){
    unused=new entry(new T());  //create first one
    mine.push_back(unused->that);
    for (int i=0; i<size-1; i++){
      entry* e=new entry(new T());
      e->next=unused;
      unused=e;
      mine.push_back(unused->that);
    }
  }
  
  ~ExtensionPool(){
    //delete all T* that belong to this pool
    for (unsigned int i=0; i<mine.size(); i++){
      delete mine[i];
    }
    
    //delete all unused elements
    while (unused){
      entry* e=unused;
      unused=unused->next;
      delete e;
    }

    //delete all used elements
    while (used){
      entry* e=used;
      used=used->next;
      delete e;
    }
  }
  
  bool is_from(T* cont){ //slow!!!
    for (int i=0; i<mine.size(); i++){
      if (mine[i]==cont) return true;
    }
    return false;
  }
  
  T* construct(){
    entry* e;
    if (unused==NULL){
      e=new entry(new T());
      mine.push_back(e->that);
    }
    else{
      e=unused;
      unused=unused->next;
    }
    e->next=used;
    used=e;
    return used->that; //if all elements of pool are used, just create a new one and go on      
  }

  void free (T* cont){
    assert(used);
    entry* e=used;
    used=e->next;
    e->that=cont;
    e->next=unused;
    unused=e;
  }
  
private:
  entry* unused;
  entry* used;
  std::vector<T*> mine; //just for clean up and is_from
};

#endif

