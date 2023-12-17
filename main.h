/*

   Confereance has: 
   + translators
   + members

   members speak different languages
   spread members in a way where all
   members get translator that speak
   thier native language

   goal to pair them the most efficient
   way possible involving as less, of 
   translators, as possible;

*/

//BUILD PARAM
#pragma once

/*
      RENDER DEPENDECY
*/

#include <raylib.h>

/*
      LIBC MODULES
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
    CROSSFILE CONSTANTS
*/

#define MEMBER 0
#define TRANSL 1

#define EMPTY 0 
#define ERR_NOELEMENTS 0
#define ERR_NOMATCH -1
#define SUCCESS 1

#define NOPAIR 0
#define VEC_TERMINATOR -1

#define LANGS_COUNT 5


/*
    TYPES
*/


enum ROLE
{
  Translator = TRANSL,
  Member     = MEMBER
};

enum LANGUAGES
{
  English=1,
  Italian=2,
  Spanish=3,
  French =4,
  Russian=5,
};


typedef struct Node
  {
    uint              index;
    size_t            langs_count;
    int               languages[5];
    int               role;
    struct Node*      pair;
  }
Node;

typedef struct
  {
    uint    last;
    size_t  set_length;
    Node**  vec;
    size_t* dead_indexes;
    size_t  dead_index_count;
  }
Set;

typedef struct
  {
    Color  Background;
    Color  Foreground;
    Color  AcentColor;
    Color  DecntColor;
    Color  FontColor;
    Color  Outline;
    
    size_t font_size;
    bool   overflow;
  }
Style;

typedef struct 
  {
    Vector2 pos;
    Vector2 size;
    Color   clr_bg;
    Color   clr_outl;
    Color   clr_font;
    char*   label;
  }
Button;


typedef struct 
  {
    size_t    small_circle;
    size_t    big_circle;
    size_t    element_count;
    Set*      Members;
    Vector2*  Member_Points;
    
    Style     Draw_Style;
    Vector2   g_origin;
    Vector2   g_size;
  }
Graph;

typedef struct
  {
    int langs[5];
    int role[2]; 
  }
Params;

/*
    FUNCTIONS
*/


//  util.c
void      linkTest      (void); // does nothing except linking test
char*     buildParamStr (Params p);
void      freeStrVec    (char** vec, size_t vec_size);
int*      getDigitData  (char* token_name,char* line,size_t returned_buffer_size);
Set*      parseSet      (char** lines_stream,size_t stream_size, bool log);
uint      SharedLang    (Node member1, Node member2);
// grouping algorithm itself
  int     groupUpByLang (Set* s);


// fileread.c
size_t    linesInFile   (FILE *fptr);
char**    readFile      (FILE* fptr, size_t file_size);
int       debugPrint    (char** vec,size_t size);
int       appendWith    (char* str);

//  set.c 
bool      deadinSet     (uint index, Set* ptr);
Set*      setAlloc      (size_t set_len);
int       setRealloc    (Set* ptr, size_t newlen);
int       setDelete     (Set* ptr, uint pos);
int       setFree       (Set* ptr);
int       setInsert     (Set* set, Node* member);
int       setPointer    (Node* child,Node* parent);
int       setEnumerate  (Set* s);

//  ui.c // todo
int       processRightClk(Graph g);
void      drawLangsColors(Color langs[5], Vector2 origin, Vector2 size,Style s);
void      drawLangsAdd   (Color langs[5], Node* newmem, bool* flush, Vector2 origin, Vector2 size,Style s,Style box[2]);
void      handleAsCheck  (Button b, Style def,Style hover,Style pressed, bool *state);
Color     rgb            (short r, short g, short b);
Button    newButton      (char* label,Vector2 pos, Vector2 size, Style s);
Button    swapStyleButton(Button b,Style s);
Graph     newGraph       (Set* source,  Vector2 pos, Vector2 size, Style s);
int       drawHelp       (Vector2 origin, Vector2 size, Style menu);
int       drawGraph      (Graph g, Color langs_colors[5]);
int       drawButton     (Button b);
int       drawNode       (Node* n,Vector2 pos,size_t size, Color outline, Color fill[5]);
int       drawNodeWheel  (Node* n,Vector2 pos, size_t size, Color outline, Color fill[5]); 
int       connectNodes   (Set* s, Color line); 
bool      hoveredButton  (Button b);
bool      pressedButton  (Button b);
