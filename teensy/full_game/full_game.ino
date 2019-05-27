

//#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>  //lol yes we're using dot star and neopixel the absolute madman
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

#include "src/Adafruit_DotStar/Adafruit_DotStar.h" 
#include "src/ColorHolder/ColorHolder.h" 
#include "src/JoinArea/JoinArea.h" 

//global defines
#define NUM_COLS 96
#define NUM_ROWS 5

#define NUM_OBSTACLES 8
#define MAX_NUM_PLAYERS 3
#define MAX_TRAILS 3

#define LETTER_WIDTH 5

#define STATE_INTRO     0
#define STATE_JOIN      1
#define STATE_PREGAME   2
#define STATE_GAME      3
#define STATE_GAMEOVER  4
#define STATE_SETTINGS  5

#define NUM_BUTTONS 8

#define NEO_PIXEL_PIN   8


boolean use_debug_serial_display = false;
boolean debug_skip_intro = false;
boolean debug_no_death = false;

//need these function prototypes or else it gets mad about "ColorHolder"
void printWord(String word, ColorHolder * col, int start_x);
void printWord(String word, ColorHolder * col, int start_x, bool wrap);
void printLetter(char thisChar, ColorHolder * col, int start_x, bool wrap);

