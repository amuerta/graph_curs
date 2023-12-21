#include "main.h"




Set*     setAlloc      (size_t set_len)
{
  Set* set;
  set = (Set *) malloc( sizeof(Set) * 1 );

  set->dead_index_count = 0;
  set->dead_indexes = (size_t*) malloc(sizeof(size_t) * set_len);
  
  set->vec = (Node**) malloc( sizeof(Node*) * set_len );
  set->set_length = set_len;
  
  set->last = 0;

  for(uint i = 0; i < set_len; i++)
    set->vec[i] = (Node*) malloc(sizeof(Node)*1);

  return set;
}


int       setRealloc    (Set* set, size_t newlen)
{
  size_t* dead_indexes_cap = (size_t*) 
    realloc(set->dead_indexes,sizeof(size_t) * newlen);
  
  Node** newvec = (Node**) 
    realloc(set->vec, sizeof(Node*) * newlen);
  
  if(!newvec) return 0;
  set->vec = newvec;
  set->dead_indexes = dead_indexes_cap;

  /*
  for(uint i = 0; i < set->set_length; i++)
  {
    Node* newnode = (Node*) realloc(set->vec[i], sizeof(Node)*1);
    if (!newnode) return 0;
    set->vec[i] = newnode;
  }
 */ // turns out this is unessacery
  if(newlen > set->set_length)
    for(uint i = set->set_length; i < newlen; i++)
    {
      set->vec[i] = NULL;
      set->dead_indexes[set->dead_index_count] = i;
      set->dead_index_count++;
    }

  set->set_length = newlen;
  return 1;
}



bool  deadinSet    (uint index, Set* ptr)
{
  for(uint i = 0; i < ptr->dead_index_count; i++)
  {
    if (index == ptr->dead_indexes[i]) return true;
  }
  return false;
}


int       setDelete     (Set* set, uint pos)
{
  if (pos >= set->set_length)
    return 0;
  else if (set->set_length == 1)
    return 0;

  set->dead_indexes[set->dead_index_count] = pos;
  set->dead_index_count++;
  
  free(set->vec[pos]);
  set->vec[pos] = NULL;
  return 1;
}

int       setFree       (Set* set)
{
  int nullptrerr = (set!=0) ? 1 : 0;

  for(uint i = 0; i < set->set_length; i++) {
    if(set->vec[i]) 
      free(set->vec[i]);
  }
  free(set->dead_indexes);
  free(set);

  return nullptrerr;
}

int       setInsert     (Set* set, Node* element)
{
  const int INSERTIONFALIURE = 0;
  const int OK = 1;

  if (element->langs_count == 0)
  {
    free(element);
    return INSERTIONFALIURE;
  }

  if (set->last < set->set_length)
    if(set->dead_index_count == 0)
      set->vec[set->last++] = element;
    else {
      set->vec[set->dead_indexes[set->dead_index_count-1]] = element;
      set->dead_index_count--;
    }
  else 
    return INSERTIONFALIURE;
  
  return OK;
}


int       setPointer    (Node* child,Node* parent)
{
  if(parent != 0 && sizeof(*parent)==sizeof(Node))
    child->pair = parent;
  else 
    return 0;
  
  return 1;
}


int       setEnumerate  (Set* s)
{
  for(uint i = 0; i < s->set_length; i++)
    if(s->vec[i] != 0) s->vec[i]->index = i;
  return 1;
}
