#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>  //lol yes we're using dot star and neopixel the absolute madman
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

#define NUM_COLS 96
#define NUM_ROWS 5

#define NUM_OBSTACLES 8
#define MAX_NUM_PLAYERS 3
#define MAX_TRAILS 3

#define LETTER_WIDTH 5

#define STATE_INTRO 0
#define STATE_PREGAME 1
#define STATE_GAME 2
#define STATE_GAMEOVER 3
#define STATE_SETTINGS 4

#define NUM_BUTTONS 8

#define NEO_PIXEL_PIN   8


