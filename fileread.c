#include "main.h"


size_t    linesInFile   (FILE *fptr)
{
  size_t lines = 0;
  char c = 0;

  rewind(fptr);

  while((c = fgetc(fptr)) != EOF)
    {
      if (c == '\n') lines++;
    }
  return lines;
}

char**    readFile      (FILE* fptr, size_t file_size)
{
  rewind(fptr);
  const char NEWLINE = '\n';
  size_t len = 2;
  size_t pos = 0;
  char c = 1;
  char* str;
  char** vec;
  
  vec = (char**) malloc(sizeof(char*) * file_size);

  for(uint i = 0; i < file_size; i++) {
    vec[i] = (char*) malloc(sizeof(char*) * 1);
    vec[i][0] = '\0';
  }
  //str = malloc(sizeof(char*)*1);
  //str = memset(str,0,1);

  while((c = fgetc(fptr)) != EOF )
    if (c == NEWLINE || c == EOF)
    {
      //vec[pos] = str;
      pos++;

      //str = malloc(sizeof(char*)*1);
      //str = memset(str,0,1);
      
    }
    else {
      char* newstr;
      newstr = realloc(vec[pos], strlen(vec[pos])+2);
      vec[pos] = newstr;
     sprintf(vec[pos],"%s%c", vec[pos],c);
    }
  
  return vec;
}




int       debugPrint    (char** vec,size_t size)
{
  for(size_t i = 0; i < size; i++)
  {
    printf("string [%ld] : {%s}\n",i,vec[i]);
  }
  return 1;
}
