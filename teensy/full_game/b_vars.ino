//audio
wavTrigger wTrig;             // wav trigger object
int wav_start = 1;  //index of start jingle track (when a session begins, before players join)
int wav_join = 2;   //index of player join track
int wav_begin = 3;  //index of begin race track
int wav_shift = 4;  //index of shift obstacle track
int wav_death = 5;  //index of death track
int wav_win = 6;    //index of trakc that plays when a player wins
int wav_tick = 7;    //index of tick tock sound
int wav_reverse = 8;
int wav_accelerate = 9;
int wav_shift_lanes = 10;

int wav_countdown_a = 11;
int wav_countdown_b = 12;

bool can_play_shift_sound = false;  //there is a period during the pregame where players can start pressing the buttons

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
DeathEffect death_effects[MAX_NUM_DEATH_EFFECTS];

int winner = -1;
int gameState = STATE_INTRO;

int deltaMillis;
int prev_frame_millis;

//timing out game over
int max_game_over_time = 15000;
int end_game_over_time = 0;
int game_over_time = 0;

int deathAnimStepTime = 150;

//maintaining pixels
ColorHolder pixel[NUM_COLS][NUM_ROWS];

ColorHolder testo;

//pregame intro
int pregameStep = 0;
int nextPregameStepTime = 0;
int pregameTimer = 0;

//using serial for a debug display in processing
String debug_display_buffer = "";       //buffer size maxes out at 64 bytes, our messages can be up to 8 chars long, so max of 8 pixels per message
int num_queued_debug_display = 0;

//buttons
int debounce_time = 50; //millis

Button buttons[NUM_BUTTONS];

int button_lock_timer = 0;
int button_lock_time = 1000;

//join screen
int join_screen_start_time;
float join_screen_time_limit = 7;     //time before game starts automaticly in seconds
float join_screen_idle_timeout = 60;  //in seconds
int join_screen_end_timer;
//bool player_joined[MAX_NUM_PLAYERS];
JoinArea join_areas[MAX_NUM_PLAYERS];

int join_screen_tick_timer = 0;

//going to settings mode
int settings_timer = 0;
int settings_timer_to_trigger = 150;

int global_brightness_setting = 5;    //this is an int value that gets cycled in settings mode
float global_brightness =  1.0f;

int global_volume_setting = 4;

//Neopixel lights in the buttons

Adafruit_NeoPixel button_pixels = Adafruit_NeoPixel(NUM_BUTTONS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//LEDs
Adafruit_DotStar leds[] = {
  Adafruit_DotStar(NUM_COLS, 4, 5 , DOTSTAR_BRG),
  Adafruit_DotStar(NUM_COLS, 6, 7, DOTSTAR_BRG),
  Adafruit_DotStar(NUM_COLS + 1, 11, 12, DOTSTAR_BRG),
  Adafruit_DotStar(NUM_COLS + 1, 13, 14, DOTSTAR_BRG),  //right now this strip is messed up and we skip the first LED
  Adafruit_DotStar(NUM_COLS + 1 , 15, 16, DOTSTAR_BRG) //there was an extra LED on this one
};

//trakcing which columns have changed this frame
int updatedCols[10];
int numUpdatedCols = 0;

//game colors
ColorHolder blank_col;
ColorHolder blocker_col;
ColorHolder shifter_col;
ColorHolder accelerator_col;
ColorHolder reverse_col;
