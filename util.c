#include "main.h"


void  linkTest      (void) // for link testing only
{
  printf("\n\t[modules linked sucessfuly]\n\n");
}


void      swap          (void* item1, void* item2,size_t t_len)

{
  unsigned char * p = item1, * q = item2, tmp;
  for (size_t i = 0; i != t_len; ++i)
  {
    tmp = p[i];
    p[i] = q[i];
    q[i] = tmp;
  }
}

void      freeStrVec    (char** vec, size_t vec_size)
{
  for(size_t i = 0; i < vec_size; i++)
    free(vec[i]);
  
  free(vec);
}


int*      getDigitData (char* token_name,char* line,size_t returned_buffer_size)
{
  uint items = 0;
  bool read  = false;
  bool valid_id = false;
  int* lang_array = malloc(sizeof(int)*returned_buffer_size+1);

  int id_zero = '0',
      id_five = '5';

  char* token = strstr(line,token_name); 

  if (token != NULL)
  {
    while(*(token)!='\0' && *(token)!='}')
    {
      if (*(token) == '{') 
        read = true;

      if ( *(token) >= id_zero && *(token) <= id_five  )
        valid_id = true;
      else 
        valid_id = false;

      if (
          valid_id && read &&
          *(token)!=' ' &&
          returned_buffer_size > items
          )
        //printf("[%d] ",*(token)-'0');
        lang_array[items++] = *(token) - '0';

      token++;
    }
    lang_array[items+1] = VEC_TERMINATOR;
    return lang_array;
  }
  else return NULL;
}


Set*      parseSet      (char** lines_stream,size_t stream_size, bool log)
{

  Set* conf_member = setAlloc(stream_size);

  for(size_t line = 0; line < stream_size; line++)
  {
    Node* member = malloc(sizeof(Node)*1);
    int* role  = getDigitData("[ROLE]", lines_stream[line], 2); 
    //                        ^^^^^^ ONLY ONE ROLE     !^
    int* langs = getDigitData("[LANG]", lines_stream[line], LANGS_COUNT);
    //                        ^^^^^^^ AT MOST 5 LANGS  !^^^^^^^^^^^

    if(role == NULL || langs == NULL )
    {
      if (log) printf("SKIPPING LINE [%ld], line: {%s}\n",line,lines_stream[line]);
      continue;
    }

    if (*role == TRANSL)
    {
      uint index = 0;
      member->role      = TRANSL;
      while(langs[index]!=VEC_TERMINATOR) {
        
        member->languages[index] = langs[index];
        member->langs_count = index;
        index++;
      
      }
    }
    else if (*role == MEMBER) 
    {
      uint index = 0;
      while(langs[index]!=VEC_TERMINATOR)
      {
       
        member->languages[index] = langs[index];
        member->langs_count = index;
        index++;
      
      }
      member->role         = MEMBER;
    }


    setInsert(conf_member, member);
    if (log) printf("SUCCESSFLY ADDED MEMBER [%s] with N[%ld] LANGUAGES\n",
                    (*role==TRANSL) ? "TRANSLATOR" : "MEMBER",
                    member->langs_count);
  }

  return conf_member;
}


uint    SharedLang (Node member1, Node member2)
{
  uint shared_langs = 0;
  
  for(uint ms1 = 0; ms1 < member1.langs_count; ms1++)
    for(uint ms2 = 0; ms2 < member2.langs_count; ms2++)
      if(member1.languages[ms1] == member2.languages[ms2]) 
        shared_langs++;

  return shared_langs;
}


int     groupUpByLang (Set* s)
{
  uint translators_count = 0;
  uint* translator_indexes; 

  for(uint i=0; i < s->set_length;i++)
    if(s->vec[i] == EMPTY || deadinSet(i, s))
      continue;
    else if(s->vec[i]->role == TRANSL)
      translators_count++;

  if (translators_count==0)
    return ERR_NOELEMENTS;

  translator_indexes = (uint*) malloc(sizeof(uint)*translators_count);

  uint indexes=0;
  for(uint i=0; i < s->set_length;i++)
    if(s->vec[i] != 0 && s->vec[i]->role==TRANSL)
      translator_indexes[indexes++] = i;

  for(uint i=0; i < s->set_length;i++) {

    uint max_shared_langs = 0;
    uint shared_langs_count = 0;
    int best_match = -1;

    if (s->vec[i] == EMPTY || s->vec[i]->role == TRANSL)
      continue;

    for(uint transl = 0; transl < translators_count; transl++)
    {
      shared_langs_count = SharedLang(
          *s->vec[translator_indexes[transl]],
          *s->vec[i]
        );
     
      if (shared_langs_count>max_shared_langs) {
        max_shared_langs = shared_langs_count;
        best_match = translator_indexes[transl];
      }
    }

    if (best_match!=ERR_NOMATCH)
      s->vec[i]->pair = s->vec[best_match];
    else 
      s->vec[i]->pair  = NOPAIR;
  }
  free(translator_indexes);
  return SUCCESS;
}
