#include "main.h"
#include <raylib.h>
#include <stdio.h>

Color     rgb           (short r, short g, short b)
{
  const short FULL_ALPHA = 255;
  return (Color) {r,g,b,FULL_ALPHA};
}


void      drawLangsColors(Color langs[5], Vector2 origin, Vector2 size, Style s)
{
  DrawRectangle(origin.x, origin.y,
                size.x, size.y,
                s.Background);

  const uint LANGC = 5;
  const char* LANGS_STR[] = {
    "ENGLISH",
    "ITALIAN",
    "SPANISH",
    "FRENCH" ,
    "RUSSIAN"
  };
  
  uint paddings = size.y / 10;
  uint step = ( (size.y - paddings*2) / LANGC);

  for(uint i = 0; i<LANGC; i++) {
    DrawRectangle(origin.x + size.x - 3*paddings,
                  origin.y+paddings+(step*i), 
                  step/1.5, step/1.5, 
                  langs[i]);
    DrawText(LANGS_STR[i], 
             origin.x + paddings, 
             origin.y + paddings + (step*i), 
             paddings, 
             s.FontColor);
  }

}

void      drawLangsAdd   (Color langs[5], Node* newmem, bool *flush, Vector2 origin, Vector2 size,Style s,Style box[2])
{
  static bool button_states[6] = {0,0,0,0,0,0};
  static Color Background;

  if (*flush)
    for(uint i = 0; i < LANGS_COUNT; i++)
      button_states[i] = 0;

  DrawRectangle(origin.x, origin.y,
      size.x, size.y,
      s.Background);

  const uint LANGC = 6;
  const char* LANGS_STR[] = {
    "ENGLISH",
    "ITALIAN",
    "SPANISH",
    "FRENCH" ,
    "RUSSIAN",
    "Translator"
  };

  Button add_menu_buttons[6];
  
  uint paddings = size.y / 10;
  uint step = ( (size.y - paddings*2) / LANGC);

  for(uint i = 0; i<LANGC; i++)
    add_menu_buttons[i] = newButton("v",
        (Vector2)
        {
          .x = origin.x + size.x - 3*paddings,
          .y = origin.y + paddings +  (step*i)
        }, (Vector2) 
        {
          .x = step/1.5,
          .y = step/1.5
        }, 
         box[0]
        );


  for(uint i = 0; i < LANGC; i++) 
  { 
    Button button = add_menu_buttons[i];
    if(hoveredButton(button)) 
    {
      button = swapStyleButton(button, s);
      if (pressedButton(button))
        button_states[i] = !button_states[i];
    }
    else 
      button = swapStyleButton(button, box[0]);
  }

  for(uint i = 0; i<LANGC; i++) {
    
    if(button_states[i]) {
      if (i!=5)
        box[1].Background = langs[i];
      else
        box[1].Background = rgb(255, 255, 255);

      add_menu_buttons[i] = swapStyleButton(add_menu_buttons[i], box[1]);
    }
    else 
    {
      box[0].Background = rgb(30, 30 , 30);
      add_menu_buttons[i] = swapStyleButton(add_menu_buttons[i], box[0]);
    }
    
    DrawText(LANGS_STR[i], 
        origin.x + paddings, 
        origin.y + paddings + (step*i), 
        paddings, 
        s.FontColor);
    
    drawButton(add_menu_buttons[i]);
  }

  int counter = 0;
  for(uint i = 0; i < LANGS_COUNT; i++)
  {
    if (button_states[i]) {
      newmem->languages[counter] = i+1;
      counter++;
    }
    newmem->langs_count = counter;
    newmem->role = MEMBER;
    newmem->pair = 0;
    newmem->index = 0;
  }

  if(button_states[5])
    newmem->role = TRANSL;
  else 
    newmem->role = MEMBER;
}


void      handleAsCheck   (Button b, Style def,Style hover,Style pressed, bool *state)
{
  drawButton(b);
  if (hoveredButton(b))
    swapStyleButton(b, hover);

  if (pressedButton(b))
    *state = !(*state);

  if(*state)
    swapStyleButton(b, pressed);
  else
    swapStyleButton(b, def);
}




Button    newButton     (char* l,Vector2 p, Vector2 sz, Style s) 
{
  Button b = {
    .pos = p,
    .size = sz,
    .label = l,
    .clr_bg = s.Background,
    .clr_font = s.FontColor,
    .clr_outl = s.Outline
  };
  return b;
}

Button   swapStyleButton (Button b,Style s)
{
  b.clr_outl = s.Outline;
  b.clr_font = s.FontColor;
  b.clr_bg   = s.Background;
  return  b;
}

Graph     newGraph      (Set* source,  Vector2 pos, Vector2 size, Style s) 
{
  Graph g = {
    .element_count = source->set_length,
    .Members = source,
    .Member_Points = (Vector2 *) malloc(sizeof(Vector2)*source->set_length),
    .Draw_Style = s,
    .g_size = size,
    .g_origin = pos
  };
  return g;
}

int       drawGraph     (Graph g,Color langs_color[5]) 
{
  DrawRectangle(g.g_origin.x, g.g_origin.y,
                g.g_size.x, g.g_size.y, 
                g.Draw_Style.Background);

  
  float graph_proportion_top = (1./3.);
  float graph_proportion_bot = (2./3.);
  float side_padding         = g.g_size.x / 12;

  uint dot_scale   = g.g_size.x / 100; // looks nice :)
  uint translators = 0;
  uint memebers    = 0;

  setEnumerate(g.Members);

  for(uint i = 0; i < g.Members->set_length; i++)
    if(g.Members->vec[i] == 0)
      continue;
    else if(g.Members->vec[i]->role == TRANSL)
      translators++;
    else 
      memebers++;


  uint translators_spacing = (g.g_size.x - side_padding*2) / translators;
  uint memebers_spacing = (g.g_size.x - side_padding*2) / memebers;

  Vector2 transl_origin = {
    .x = g.g_origin.x + side_padding + translators_spacing/2.,
    .y = g.g_origin.y + g.g_size.y * (graph_proportion_top)
  };

  Vector2 membrs_origin = {
    .x = g.g_origin.x + side_padding + memebers_spacing/2.,
    .y = g.g_origin.y + g.g_size.y * (graph_proportion_bot)
  };

  for(uint i = 0; i < g.Members->set_length;i++)
    if(g.Members->vec[i]==0)
      continue;
    else if(g.Members->vec[i]->role == TRANSL)
    {
      drawNodeWheel(g.Members->vec[i],
                    transl_origin, 
                    translators_spacing/dot_scale, 
                    g.Draw_Style.Outline, 
                    langs_color );
     
      g.Member_Points[i] = transl_origin;
      transl_origin.x += translators_spacing;
      g.big_circle = translators_spacing/dot_scale;
    }
    
    else
    {
      if (g.Members->vec[i]->langs_count == 1)
        drawNode(g.Members->vec[i], 
               membrs_origin,
               memebers_spacing/dot_scale,
               g.Draw_Style.Outline, 
               langs_color);
      else 
        drawNodeWheel(g.Members->vec[i], 
                      membrs_origin,
                      memebers_spacing/dot_scale,
                      g.Draw_Style.Outline, 
                      langs_color);

      
      g.Member_Points[i] = membrs_origin;
      membrs_origin.x += memebers_spacing;
      g.small_circle = memebers_spacing/dot_scale;
    }

  //
  // CONNECT THE NODES
  //
  for(uint i = 0; i < g.Members->set_length;i++)
  {
    if (g.Members->vec[i] == NULL)
      continue;
    else if (g.Members->vec[i]->pair != NOPAIR)
    {

      if (translators > 0) {

      Vector2 
        BEG = g.Member_Points[
            g.Members->vec[i]->index
          ],
        END = g.Member_Points[
            g.Members->vec[i]->pair->index
          ];

        BEG.y -= (float)memebers_spacing/dot_scale;
        END.y += (float)translators_spacing/dot_scale;

      DrawLineEx(
          BEG, 
          END, 
          2, 
          g.Draw_Style.Outline);
   /* printf("| * {%f,%f} -> {%f,%f} \n",

          g.Member_Points[
            g.Members->vec[i]->index
          ].x, 

          g.Member_Points[
            g.Members->vec[i]->index
          ].y,
 
          g.Member_Points[
            g.Members->vec[i]->pair->index
          ].x, 

          g.Member_Points[
            g.Members->vec[i]->pair->index
          ].y 
        );
    */
      }
    }
  }
  return 1;
}

int       drawButton    (Button b) 
{
  float thickness = b.size.y / 16;
  float outline_pad = thickness / 2;

  float end_x = b.pos.x + b.size.x;
  float end_y = b.pos.y + b.size.y;

  float font_size = b.size.y / 2;

  DrawRectangle(b.pos.x, b.pos.y, b.size.x, b.size.y, b.clr_bg);
  // upper
  DrawLineEx(
      (Vector2) {
        .x = b.pos.x  ,
        .y = b.pos.y + outline_pad
      }, 
      (Vector2) {
        .x = end_x,
        .y = b.pos.y + outline_pad
      }, 
      thickness, 
      b.clr_outl);
   // lower
  DrawLineEx(
      (Vector2) {
        .x = b.pos.x  ,
        .y = end_y - outline_pad
      }, 
      (Vector2) {
        .x = end_x,
        .y = end_y - outline_pad
      }, 
      thickness, 
      b.clr_outl);
  // left
  DrawLineEx(
      (Vector2) {
        .x = b.pos.x + outline_pad  ,
        .y = b.pos.y
      }, 
      (Vector2) {
        .x = b.pos.x + outline_pad,
        .y = end_y - outline_pad
      }, 
      thickness, 
      b.clr_outl);

  // right
  DrawLineEx(
      (Vector2) {
        .x = end_x - outline_pad  ,
        .y = b.pos.y
      }, 
      (Vector2) {
        .x = end_x - outline_pad,
        .y = end_y - outline_pad
      }, 
      thickness, 
      b.clr_outl);


  float font_thickness = font_size * 0.5;

  float label_x = b.pos.x + (b.size.x - strlen(b.label) * font_thickness ) / 2;
  float label_y = b.pos.y + (b.size.y - font_size) / 2;

  DrawText(b.label, 
      label_x, 
      label_y,
      font_size, 
      b.clr_font
    );

  return 1;
}


bool      hoveredButton (Button b)
{
  Vector2 cursor_pos = 
  { 
    .x = GetMouseX(), 
    .y = GetMouseY() 
  };
 
  bool
    before_x = cursor_pos.x < b.pos.x,
    before_y = cursor_pos.y < b.pos.y,

    after_x  = cursor_pos.x > b.pos.x + b.size.x,
    after_y  = cursor_pos.y > b.pos.y + b.size.y
    ;

  bool 
    within_x = !before_x && !after_x,
    within_y = !before_y && !after_y;

  return ( within_x && within_y );
}


bool      pressedButton (Button b) 
{
  if (
      hoveredButton(b) &&
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
     ) return true;
  return  false;
}

int       drawNode      (Node* n,Vector2 pos, size_t size, Color outline, Color fill[5]) 
{
  Color language_color = fill[ n->languages[0] - 1];
// languages enum 1..5 fill indexes 0..4 thus  ^^^
  DrawRing(pos, 0, size, 0, 360, 10, language_color);
  DrawRingLines(pos, 0, size, 0, 360, 10, outline);
  return 1;
}


int       drawNodeWheel  
  (Node* n,Vector2 pos, size_t size, Color outline, Color fill[5])
{
  const float FULL_CIRCLE = 360;

  if (n->langs_count>5)
    return 0; // error

  
  float slice_size = FULL_CIRCLE / n->langs_count;
  float origin = 0;

  for(uint i = 0; i < n->langs_count; i++)
  {
    DrawRing(pos, 0, size,
             origin, 
             origin+slice_size, 
             10, 
             
             fill[ n->languages[i] - 1 ] 
// fill color at index of color id (enum LANGYAGES) - 1 
// same as drawNode(Node*,Vec2,size_t,Color[5]) tech.
    );
    origin+=slice_size;
  }
  return  1;
}



int      processRightClk(Graph g)
{
  Vector2 mouse = { .x =  GetMouseX(), .y = GetMouseY() };

    for(uint i = 0; i < g.Members->set_length; i++)
    {
      size_t selector_size = 10;//(g.Members->vec[i]->role == TRANSL)? g.big_circle : g.small_circle; 

      //printf("cord [%d] : {%f , %f}\n",i,g.Member_Points[i].x,g.Member_Points[i].y);

      bool within_x =
        g.Member_Points[i].x - selector_size < mouse.x &&
        g.Member_Points[i].x + selector_size > mouse.x;

      bool within_y =
        g.Member_Points[i].y - selector_size < mouse.y &&
        g.Member_Points[i].y + selector_size > mouse.y;


      if(within_x && within_y)
      {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && g.Members->vec[i] != NULL) 
          setDelete(g.Members, g.Members->vec[i]->index);
      }
      else
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
  return 1;
}


int       drawHelp       (Vector2 origin, Vector2 size, Style menu)
{

  float thickness = size.y / 16;
  float outline_pad = thickness / 2;

  float end_x = origin.x + size.x;
  float end_y = origin.y + size.y;

  float font_size = size.y / 2;

  // BACKGROUND

  DrawRectangle(origin.x, origin.y,
                size.x, size.y, 
                menu.Background);
  // OUTLINE 

  // upper
  DrawLineEx(
      (Vector2) {
        .x = origin.x  ,
        .y = origin.y + outline_pad
      }, 
      (Vector2) {
        .x = end_x,
        .y = origin.y + outline_pad
      }, 
      thickness, 
      menu.Outline);
   // lower
  DrawLineEx(
      (Vector2) {
        .x = origin.x  ,
        .y = end_y - outline_pad
      }, 
      (Vector2) {
        .x = end_x,
        .y = end_y - outline_pad
      }, 
      thickness, 
      menu.Outline);
  // left
  DrawLineEx(
      (Vector2) {
        .x = origin.x + outline_pad  ,
        .y = origin.y
      }, 
      (Vector2) {
        .x = origin.x + outline_pad,
        .y = end_y - outline_pad
      }, 
      thickness, 
      menu.Outline);

  // right
  DrawLineEx(
      (Vector2) {
        .x = end_x - outline_pad  ,
        .y = origin.y
      }, 
      (Vector2) {
        .x = end_x - outline_pad,
        .y = end_y - outline_pad
      }, 
      thickness, 
      menu.Outline);


  // draw text
  
  short lines = 19;

  short text_size = size.y / (lines + 2);

  const char* LINES[] = 
  {
    "HELP: ",
    " Program draw tree-like reloational graph with",
    " color wheels displaying entity propeties.",
    "",
    " Enteties in the program have two types:",
    "  Translators, or Members, each can know",
    "  Up to 5 languages.",
    "",
    " Matching Alghorithm will connect Memeber",
    " to first most optimal traslator.",
    "",
    " This ensures least amount of tramslators",
    " being involved",
    "",
    "NOTE: ",
    " ALL GRAPH DATA IS SOURCED FROM A USER-MADE",
    " FILE, THAT BEING SPECIFIED AS ONLY PROGRAM",
    " ARGUMENT.",
    "EXAMPLE CAN BE VIEWED WITH `--help`  FALG"
  };

  for(uint i = 0; i < lines; i++)
  {
    DrawText(LINES[i],
             origin.x + outline_pad + text_size,
             origin.y + outline_pad + text_size + (text_size*i), 
             text_size, 
             menu.FontColor);
  }

  return 1;
}


int       drawAdd        
  (Graph g, Node* element, Vector2 origin, Vector2 size, Style menu,Style highlight, Color langs[5],bool *switchoff)
{
  
  float thickness = size.y / 16;
  float outline_pad = thickness / 2;

  float end_x = origin.x + size.x;
  float end_y = origin.y + size.y;

  float font_size = size.y / 2;


  Button BTNS[5];
  
  bool ACTIVE[5] = {0,0,0,0,0};

  char *LABELS[5] =
  {
    "English",
    "Italian",
    "Spanish",
    "French",
    "Russian"
  };

  // BACKGROUND

  DrawRectangle(origin.x, origin.y,
                size.x, size.y, 
                menu.Background);
  
  drawNodeWheel(element, 
      (Vector2)
      {
      .x = origin.x + size.x * 0.75,
      .y = origin.y + size.y * 0.50
      }, 
      size.y * 0.3,
      menu.Foreground,
      langs);



  // OUTLINE 

  // upper
  DrawLineEx(
      (Vector2) {
        .x = origin.x  ,
        .y = origin.y + outline_pad
      }, 
      (Vector2) {
        .x = end_x,
        .y = origin.y + outline_pad
      }, 
      thickness, 
      menu.Outline);
   // lower
  DrawLineEx(
      (Vector2) {
        .x = origin.x  ,
        .y = end_y - outline_pad
      }, 
      (Vector2) {
        .x = end_x,
        .y = end_y - outline_pad
      }, 
      thickness, 
      menu.Outline);
  // left
  DrawLineEx(
      (Vector2) {
        .x = origin.x + outline_pad  ,
        .y = origin.y
      }, 
      (Vector2) {
        .x = origin.x + outline_pad,
        .y = end_y - outline_pad
      }, 
      thickness, 
      menu.Outline);

  // right
  DrawLineEx(
      (Vector2) {
        .x = end_x - outline_pad  ,
        .y = origin.y
      }, 
      (Vector2) {
        .x = end_x - outline_pad,
        .y = end_y - outline_pad
      }, 
      thickness, 
      menu.Outline);

  Vector2 buttons_origin = {
    .x = origin.x + size.x / 10,
    .y = origin.y + size.y / 10
  };

  Vector2 buttons_size = {
    .x = size.x / 3,
    .y = size.y / 12
  };

  char* text = "TO ADD MORE ELEMENTS, EDIT 'DATA' FILE.";
  uint text_len = strlen(text);
  uint text_height = size.y / text_len * 2.;

  DrawText(text, 
      origin.x + (size.x / 2 - ((text_len / 2.) *0.5*  text_height)) - 10,
      origin.y + size.y / 2 - text_height / 2., 
      text_height,
      menu.FontColor);

  Button subm = 
    newButton("OK",
        (Vector2)
        {
        .x = origin.x + (size.x/2 - buttons_size.x/2),
        .y = buttons_origin.y + (8 * buttons_size.y)
        }, 
        buttons_size, 
        menu);
  if (hoveredButton(subm)) {
    subm = swapStyleButton(subm, highlight);
    if(pressedButton(subm))
      *switchoff = false;
  }
  else
    subm = swapStyleButton(subm, menu);

  drawButton(subm);





  return 1;
}
