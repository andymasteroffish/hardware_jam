

boolean use_debug_serial_display = false;
boolean debug_skip_intro = false;
boolean debug_no_death = false;

//game values
int startSpeed = 230;  //measured in millis between steps
float speedMult = 0.75;  //the closer this is to 0, the faster they'll go
float slowDownMult = 1.5; //if we have one of the obstacles slow the players down, this is how much it will slow them

int num_players = MAX_NUM_PLAYERS;

//speeding up over time
int millis_between_speed_up = 1000;
int next_speed_up_time = 0;
int speed_up_ammount = 2;//5;


Obstacle obstacles[NUM_OBSTACLES];
Player players[MAX_NUM_PLAYERS];
int playerStarts[MAX_NUM_PLAYERS];

int winner = -1;
int gameState = STATE_INTRO;

int delta_millis;
int prev_frame_millis;

//timing out game over
int max_game_over_time = 15000;
int end_game_over_time = 0;
int game_over_time = 0;

int deathAnimStepTime = 150;

//maintaining pixels
char pixel[NUM_COLS][NUM_ROWS];
char last_sent_grid[NUM_COLS][NUM_ROWS];

//pregame intro
int pregameStep = 0;
int nextPregameStepTime = 0;

//using serial for a debug display in processing
String debug_display_buffer = "";       //buffer size maxes out at 64 bytes, our messages can be up to 8 chars long, so max of 8 pixels per message
int num_queued_debug_display = 0;

//buttons
int debounce_time = 50; //millis

Button buttons[NUM_BUTTONS];

int button_lock_timer = 0;
int button_lock_time = 1000;

//join screen
int join_sreen_start_time;
int join_screen_time_limit = 5000;
bool player_joined[MAX_NUM_PLAYERS];
int join_screen_end_timer;

//going to settings mode
int settings_timer = 0;
int settings_timer_to_trigger = 150;

int global_brightness_setting = 5;    //this is an int value that gets cycled in settings mode
float global_brightness =  1.0f;

//Neopixel lights in the buttons

Adafruit_NeoPixel button_pixels = Adafruit_NeoPixel(NUM_BUTTONS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//LEDs
//this is ugly, but putting them in an array from the start keeps failing
//data pin then clock pin
//data is green, clock is yellow
Adafruit_DotStar pix0 = Adafruit_DotStar(NUM_COLS, 4, 5 , DOTSTAR_BRG);
Adafruit_DotStar pix1 = Adafruit_DotStar(NUM_COLS, 6, 7, DOTSTAR_BRG);
Adafruit_DotStar pix2 = Adafruit_DotStar(NUM_COLS + 1, 11, 12, DOTSTAR_BRG);
Adafruit_DotStar pix3 = Adafruit_DotStar(NUM_COLS + 1, 13, 14, DOTSTAR_BRG);  //right now this strip is messed up and we skip the first LED
Adafruit_DotStar pix4 = Adafruit_DotStar(NUM_COLS + 1 , 15, 16, DOTSTAR_BRG); //there was an extra LED on this one

//trakcing which columns have changed this frame
int updatedCols[10];
int numUpdatedCols = 0;
