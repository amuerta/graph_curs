

/*
   modules 
*/


#include "main.h"
#include "ui.c"
#include "util.c"
#include "set.c"
#include "fileread.c"

// compile time constants

#define WIN_HEIGHT 500
#define WIN_WIDTH  900
#define TITLE      "MATCHING"
#define FPS        30

const Style APP_STYLE =
{
  .Background = {50,50,50},
};



int main(int argc, char** argv)
{

  linkTest();


  // 
  // Styles
  //


  Style NORMAL = {
    .Background = rgb(70,70,70),
    .Outline    = rgb(255,255,255),
    .FontColor  = rgb(255,255,255)
  };

  Style BRIGHT = {
    .Background = rgb(240,240,150),
    .Outline    = rgb(0,0,0),
    .FontColor  = rgb(0,0,0)
  };

  Style BOX_ON = {
    .Background = rgb(255,255,255),
    .Outline    = rgb(255,255,255),
    .FontColor  = rgb(5,5,5)
  };

  Style BOX_OFF = {
    .Background = rgb(30,30,30),
    .Outline    = rgb(255,255,255),
    .FontColor  = rgb(30,30,30)
  };

  Style BOXES[2] = {
    BOX_OFF,BOX_ON 
  };

  Style GRAPH = {
    .Background = rgb(45, 40, 40),
    .Outline    = rgb(255, 255, 255),
    .FontColor  = rgb(200, 200, 200)
  };

  Style CheckboxState[3] = {
    GRAPH,
    NORMAL,
    BRIGHT
  };

  Color LANGUAGES_COLORS[5] = {
    rgb(255, 26, 0), // RED
    rgb(255, 141, 0),// ORANGE
    rgb(227, 255, 0),// YELLOW
    rgb(0, 255, 4),  // GREEN
    rgb(0, 81, 255), // BLUE
  };



  // data

  FILE *f;
  char** line_vec;
  size_t file_size;
 
  Set *conf_memeber;

  if (argc > 1) {
    f = fopen("data", "r");
  }
  else 
  {
    printf("WARN: NO FILE WHERE SPECIFIED");
    exit(-1);
  }
  
  if (f==NULL) 
  {
    printf("ERR: FILE NOT FOUND\n");
    exit(-1);
  }

  file_size = linesInFile(f);
  line_vec = readFile(f, file_size);


  debugPrint(line_vec, file_size);

  conf_memeber = parseSet(line_vec, file_size, true);

  groupUpByLang(conf_memeber);


  fclose(f);

  //      UI STUFF
 
  // ui scaling
  /*
    +---+-------------+
    |   |             |
    |   |             |
    |   |             |
    |   |             |
    |   |             |
    +---+-------------+
    BUTTONS
    <---> 
            GRAPH
         <------------>
  */

  uint region_padding = 20; // px

  Vector2
    region_buttons_origin = {
                             0+region_padding,
                             0+region_padding
                            },
    region_buttons_size   = {
                              (WIN_WIDTH*(2./5.))-region_padding*2,
                              (WIN_HEIGHT )-region_padding*2
                            },
  
    region_graph_origin   = {
                              0+region_padding*2 + region_buttons_size.x,
                              0+region_padding
                            },
    region_graph_size     = {
                              (WIN_WIDTH*(3./5.))-region_padding*2,
                              (WIN_HEIGHT)-region_padding*2,
                            },
    subreg_buttons_size   = {
                              region_buttons_size.x,
                              region_buttons_size.y * (2./3.)
                            },
    subreg_help_origin  = {
                              region_buttons_origin.x  ,
                              region_buttons_origin.y + subreg_buttons_size.y + 
                                ( region_buttons_size.y * (1./3.) ) / 4.
                            },
    subreg_help_size    = {
                              region_buttons_size.x,
                              ( region_buttons_size.y / 2 * (1./3.) ) / 2.
                            },
    subreg_add_origin  = {
                              region_buttons_origin.x,
                              region_buttons_origin.y + subreg_buttons_size.y + 
                                ( region_buttons_size.y * 2.5 * (1./3.) ) / 4.
                            },
    subreg_add_size    = {
                              region_buttons_size.x,
                              ( region_buttons_size.y / 2 * (1./3.) ) / 2.
                            },
    menu_origin           = {
                              WIN_WIDTH / 10.,
                              WIN_HEIGHT / 10.
    },
    menu_size             = {
                              WIN_WIDTH - 2*(WIN_WIDTH/10.),
                              WIN_HEIGHT - 2*(WIN_HEIGHT/10.)
                            },
    ok_origin             = {
                              WIN_WIDTH * (3.25/5.),
                              WIN_HEIGHT * (1./5.)
                            },
    ok_size               = {
                              WIN_WIDTH * (1./5.),
                              WIN_WIDTH * (1./5.),
                            },
    wheel_origin          = {
                              region_graph_origin.x + region_graph_size.x / 2,
                              region_graph_origin.y + region_graph_size.y / 2
                            },
    wheel_size            = {
                              region_graph_size.x / 3,
                              region_graph_size.y / 3
                            }
  ;
 
  Node* newmember;
  Params new_node_settings;
  bool ADD_HANDLER = false;
  bool ADD   = false;
  bool HELP  = false;
  // init window

  InitWindow(WIN_WIDTH, WIN_HEIGHT, TITLE);
  SetTargetFPS(FPS);
 
  Button add_menu = newButton("ADD NEW ELEMENT", subreg_add_origin, subreg_add_size, NORMAL);
  Button help     = newButton("ABOUT PROGRAM",subreg_help_origin, subreg_help_size, NORMAL);
 
  Button add  = newButton("Add this member", subreg_add_origin, subreg_add_size, NORMAL);
  Button back = newButton("<< [ Go back ] ",subreg_help_origin, subreg_help_size, NORMAL);
 
  Button help_ok = newButton("OK",ok_origin, ok_size, NORMAL);
 
  Graph g = newGraph(conf_memeber, region_graph_origin,region_graph_size, GRAPH);



  while (!WindowShouldClose()) {
   
   BeginDrawing();
   ClearBackground(APP_STYLE.Background);
   
   if (ADD)
   { 

     drawButton(add);
     drawButton(back);
     drawLangsAdd   
       (LANGUAGES_COLORS,  newmember, &ADD_HANDLER, region_buttons_origin,subreg_buttons_size,NORMAL,BOXES); 
     
     if(newmember->langs_count!=0||newmember!=0)
       drawNodeWheel
         (newmember, wheel_origin, wheel_size.x, NORMAL.FontColor, LANGUAGES_COLORS);

     if(hoveredButton(add)) 
     {
       add = swapStyleButton(add, BRIGHT);
       if (pressedButton(add)) {
         
         ADD = !ADD;
         
         if (newmember->langs_count == 0) {
           if(newmember) free(newmember);
           newmember = 0;
         }
         else {
           int inserted = setInsert(g.Members, newmember);
           if (inserted)
             newmember = 0;
           else {
             setRealloc(g.Members, g.Members->set_length+1);
             setInsert(g.Members, newmember);
             newmember = 0;
           }
         }

       }
     }
     else 
       add = swapStyleButton(add, NORMAL);


     if(hoveredButton(back)) 
     {
       back = swapStyleButton(back, BRIGHT);
       if (pressedButton(back)) {
         ADD = !ADD;
         free(newmember);
         newmember = 0;
       }
     }
     else 
       back = swapStyleButton(back, NORMAL);


     //drawAdd(g, newmember, menu_origin, menu_size, NORMAL,BRIGHT, LANGUAGES_COLORS, &ADD);
   }
   else if (HELP)
   {
     if(hoveredButton(help_ok)) 
     {
       help_ok = swapStyleButton(help_ok, BRIGHT);
       if (pressedButton(help_ok))
         HELP = false;
     }
     else 
       help_ok = swapStyleButton(help_ok, NORMAL);



     drawHelp(menu_origin,
         menu_size, 
         NORMAL);
     drawButton(help_ok);
   }


   else
   {
     if (hoveredButton(add_menu))
     {
       add_menu = swapStyleButton(add_menu, BRIGHT);
       if (pressedButton(add_menu)) {
         ADD = true;
         newmember = (Node*) malloc(sizeof(Node));
       }
     }
     else
       add_menu = swapStyleButton(add_menu, NORMAL);



     if (hoveredButton(help))
     {
       help = swapStyleButton(help, BRIGHT);
       if (pressedButton(help))
         HELP = true;
     }
     else
       help = swapStyleButton(help, NORMAL);

     // to delete nodes
     if(processRightClk(g))
     {
       groupUpByLang(g.Members);
     }


     drawLangsColors(LANGUAGES_COLORS,region_buttons_origin,subreg_buttons_size,NORMAL);
     drawGraph(g,LANGUAGES_COLORS);
     drawButton(help);
     drawButton(add_menu);




   }
   EndDrawing();
  }


  // free resources

  //fclose(f);
  //if(newmember) free(newmember);
  setFree(conf_memeber);
  freeStrVec(line_vec, file_size);

  CloseWindow();
}
