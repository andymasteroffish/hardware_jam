#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

#define NUM_COLS 85
#define NUM_ROWS 5

#define NUM_OBSTACLES 8
#define NUM_PLAYERS 2

boolean use_debug_serial_display = false;

//game values
int startSpeed = 400;  //measured in millis between steps
float speedMult = 0.8;  //the closer this is to 0, the faster they'll go

/* obstacles can be any one of the following
  b - blocker
  s - shifter
  a - accelerator
  r - reverse       */
struct Obstacle {
  int x;
  char action;
  boolean onRows[NUM_ROWS];
};

//Player info
struct Player {
  int x;
  int y;
  int speed;          //meaured in how many millis before next move. lower val = faster player
  int nextMoveTime;   //when the next move will occur
  int dir;            //1 is normal, -1 is reverse
  char identifier;
};

//buttons
struct Button {
  int pin;
  int next_check_time;  //for debounce
  bool is_held;
  char key;
};

Obstacle obstacles[NUM_OBSTACLES];
Player players[NUM_PLAYERS];

int winner = -1;
boolean gameOver = false;
boolean gameBegun = false;

//maintaining pixels
char pixel[NUM_COLS][NUM_ROWS];
char last_sent_grid[NUM_COLS][NUM_ROWS];

//using serial for a debug display in processing
String debug_display_buffer = "";       //buffer size maxes out at 64 bytes, our messages can be up to 8 chars long, so max of 8 pixels per message
int num_queued_debug_display = 0;

//buttons
int debounce_time = 30; //millis
#define NUM_BUTTONS 8
Button buttons[NUM_BUTTONS];

//LEDs
//this is ugly, but putting them in an array from the start keeps failing
//data pin then clock pin
//data is green, clock is yellow
Adafruit_DotStar pix0 = Adafruit_DotStar(NUM_COLS, 4, 5 , DOTSTAR_BRG);
Adafruit_DotStar pix1 = Adafruit_DotStar(NUM_COLS, 6, 7, DOTSTAR_BRG);
Adafruit_DotStar pix2 = Adafruit_DotStar(NUM_COLS, 11, 12, DOTSTAR_BRG);
Adafruit_DotStar pix3 = Adafruit_DotStar(NUM_COLS, 13, 14, DOTSTAR_BRG);    //right now this strip is messed up and we skip the first LED
Adafruit_DotStar pix4 = Adafruit_DotStar(NUM_COLS, 15, 16, DOTSTAR_BRG);

//trakcing which columns have changed this frame
int updatedCols[10];
int numUpdatedCols = 0;

//timing
int frame_num = 0;

void setup() {
  Serial.begin(345600);

  Serial.write("hello");

  gameOver = false;
  gameBegun = false;
  winner = -1;

  //default states
  resetMatrix();

  //setup players
  players[0].identifier = '0';
  players[1].identifier = '1';
  //  for (int i = 0; i < NUM_PLAYERS; i++) {
  //    //players[i] = new Player();
  //
  //    //player position etc handled in reset()
  //  }

  //setup obstacles
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    //obstacles[i] = new Obstacle();
    obstacles[i].x = (NUM_COLS / NUM_OBSTACLES) * i;
    for (int r = 0; r < NUM_ROWS; r++) {
      obstacles[i].onRows[r] = false;
    }
  }

  //set the types
  //moving the X values to match the buttons
  obstacles[0].action = 'r';
  obstacles[0].x = 2;

  obstacles[1].action = 's';
  obstacles[1].x += 1;

  obstacles[2].action = 'a';
  obstacles[2].x += 0;

  obstacles[3].action = 's';
  obstacles[3].x += 5;

  obstacles[4].action = 'r';
  obstacles[4].x += 4;

  obstacles[5].action = 'b';
  obstacles[5].x += 3;

  obstacles[6].action = 'a';
  obstacles[6].x += 2;

  obstacles[7].action = 's';
  obstacles[7].x += 7;

  //dpending on the type, turn a few on
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    if (obstacles[i].action == 's') {
      obstacles[i].onRows[0] = true;
      obstacles[i].onRows[1] = true;
    }
    if (obstacles[i].action == 'a' || obstacles[i].action == 'r' || obstacles[i].action == 'b') {
      obstacles[i].onRows[0] = true;
    }
  }

  //buttons
  buttons[0].pin = 0;
  buttons[0].key = '0';

  buttons[1].pin = 1;
  buttons[1].key = '1';

  buttons[2].pin = 2;
  buttons[2].key = '2';

  buttons[3].pin = 3;
  buttons[3].key = '3';

  buttons[4].pin = 17;
  buttons[4].key = '4';

  buttons[5].pin = 18;
  buttons[5].key = '5';

  buttons[6].pin = 19;
  buttons[6].key = '6';

  buttons[7].pin = 20;
  buttons[7].key = '7';

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].next_check_time = 0;
    buttons[i].is_held = false;
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  //LEDs
  pix0.begin();
  pix0.show();

  pix1.begin();
  pix1.show();

  pix2.begin();
  pix2.show();

  pix3.begin();
  pix3.show();

  pix4.begin();
  pix4.show();

  //reset();  //testing
}

void reset() {
  if (!use_debug_serial_display) {
    Serial.println("game start");
  }

  gameOver = false;
  gameBegun = true;
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (NUM_PLAYERS == 2) players[i].y = i * (NUM_ROWS - 1); //put on opposite side
    else                  players[i].y = i; //use its own lane
    players[i].speed = startSpeed;
    players[i].dir = 1;
  }

  //set player starting positions
  players[0].x = 3;
  players[1].x = 23;

  frame_num = 0;
}

void loop() {
  checkInput();

  if (!gameBegun)      displayIntro();
  else if (!gameOver)  runGame();
  else                 displayWinner(winner);

  //displaying the thing
  setLEDs();

  if (use_debug_serial_display && num_queued_debug_display > 0) {
    sendDebugDisplayMessage();
  }

  frame_num++;
}

void runGame() {

  winner = checkWinners(); //winner remains -1 if no winner
  if (winner != -1) gameOver = true;

  //update our players
  for (int i = 0; i < NUM_PLAYERS; i++) {
    //is it time to move?
    if (millis() > players[i].nextMoveTime) {
      advancePlayer(i);
      players[i].nextMoveTime = millis() + players[i].speed;
    }
  }

  //render
  displayGame();
}

void advancePlayer(int p) {
  //move
  players[p].x += players[p].dir;
  if (players[p].x == NUM_COLS) {
    players[p].x = 0;
  }
  if (players[p].x == -1) {
    players[p].x = NUM_COLS-1;
  }

  //check for obstacles
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    if (obstacles[i].x == players[p].x) {
      boolean hitMe = false;
      for (int r = 0; r < NUM_ROWS; r++) {
        if (obstacles[i].onRows[r] && r == players[p].y) {
          //this fool hit the obsacle
          hitMe = true;
        }
      }
      if (hitMe) {
        doObstacleEffect(p, i);
      }
    }
  }
}

void doObstacleEffect(int p, int o) {
  char action = obstacles[o].action;

  //block
  if (action == 'b') {
    players[p].speed = 0;
  }

  //accelerate
  if (action == 'a') {
    players[p].speed *= speedMult;
    //keep it possitive
    if (players[p].speed < 10)    players[p].speed = 10;
  }

  //reverse
  if (action == 'r') {
    players[p].dir = -players[p].dir;
  }

  //shift
  if (action == 's') {
    //go through and find the other row that is on
    for (int r = 0; r < NUM_ROWS; r++) {
      if (players[p].y != r && obstacles[o].onRows[r]) {
        players[p].y = r;
        break;
      }
    }

  }
}

int checkWinners() { //returns winner index if won
  int winPlayer = -1;
  int playersAlive = 0;
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (players[i].speed != 0) {
      playersAlive++;
      winPlayer = i;
    }
  }
  if (playersAlive == 1) {
    //println("winner found (player " + winPlayer);
    return winPlayer;
  } else {
    return -1;
  }
}


void checkInput() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (millis() > buttons[i].next_check_time) {
      //get the current value
      bool cur_val = false;
      if (digitalRead(buttons[i].pin) == LOW) {
        cur_val = true;
      }

      //is that different form the current value?
      if (cur_val != buttons[i].is_held) {
        buttons[i].is_held = cur_val;
        buttons[i].next_check_time = millis() + debounce_time;
        //Serial.println("button: " + String(buttons[i].is_held));
        if (buttons[i].is_held) {
          button_pressed(i);
        }
      }
    }
  }
}

void button_pressed(int id) {
  if (!use_debug_serial_display) {
    Serial.print("pressed ");
    Serial.println(id);
    //return; //kill me
  }
  //reset the game if we're not playing
  if (!gameBegun || gameOver) {
    reset();
  }

  //bump this obstacle down one
  boolean temp = obstacles[id].onRows[NUM_ROWS - 1];
  for (int r = NUM_ROWS - 1; r > 0; r--) {
    obstacles[id].onRows[r] = obstacles[id].onRows[r - 1];
  }
  obstacles[id].onRows[0] = temp;
}

void displayGame() {
  resetMatrix();

  //add the obstacles
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    for (int r = 0; r < NUM_ROWS; r++) {
      if (obstacles[i].onRows[r]) {
        pixel [obstacles[i].x][r] = obstacles[i].action;
      }
    }
  }

  //add the players
  for (int i = 0; i < NUM_PLAYERS; i++) {
    pixel[players[i].x][players[i].y] = players[i].identifier;
  }
}


void displayIntro() {
  int mod = (millis() / 600) % NUM_COLS;
  String abc = "-bsar01";
  //println("mod=" + mod);

  //normal display
  if (!use_debug_serial_display) {
    for (int y = 0; y < NUM_ROWS; y++) {
      for (int x = 0; x < NUM_COLS; x++) {
        int loc = x + mod + y * NUM_COLS;
        pixel[x][y] = abc.charAt(loc % abc.length());
      }
    }
  }

  //debug display with fewer changing pixels
  else {
    for (int y = 0; y < NUM_ROWS; y++) {
      pixel[mod][y] = 'b';
    }
  }
}
void displayWinner(int player) {
  int mod = (millis() / 50) % NUM_COLS;
  //println("PLAYER " + player + " WON!!!" + " mod=" + mod);
  for (int y = 0; y < NUM_ROWS; y++) {
    for (int x = 0; x < NUM_COLS; x++) {
      int loc = x + y * NUM_COLS;
      pixel[x][y] = players[player].identifier;// Integer.toString(player).charAt(0);
      if (loc % NUM_COLS == mod || (loc + NUM_COLS/3) % NUM_COLS == mod || (loc + (NUM_COLS/3)*2) % NUM_COLS == mod ) pixel[x][y] = '-';
    }
  }
  gameOver = true;
}


void resetMatrix() {
  for (int y = 0; y < NUM_ROWS; y++) {
    for (int x = 0; x < NUM_COLS; x++) {
      pixel[x][y] = '-';
    }
  }
}

void setLEDs() {
  boolean anythingChanged = false;

  for (int y = 0; y < NUM_ROWS; y++) {
    for (int x = 0; x < NUM_COLS; x++) {

      //chekc if this pixel has changed
      if (pixel[x][y] != last_sent_grid[x][y]) {
        char col_char = pixel[x][y];
        uint32_t color = 0x000000;  //'-'

        if (col_char == 'b') color = 0x004400;  //blocked
        if (col_char == 's') color = 0x444444;  //shifter
        if (col_char == 'a') color = 0x440000;  //accelerator
        if (col_char == 'r') color = 0x004444;  //reverse

        if (col_char == '0') color = 0x000088;  //p1
        if (col_char == '1') color = 0x888800;  //p2
        if (col_char == '2') color = 0x000044;  //p3


        if (y == 0) pix0.setPixelColor(x, color);
        if (y == 1) pix1.setPixelColor(x, color);
        if (y == 2) pix2.setPixelColor(x, color);
        //row 3 is missed up so we need to skip the first pixel
        if (y == 3){ 
          if (x > 0)  pix3.setPixelColor(x-1, color);
        }
        if (y == 4) pix4.setPixelColor(x, color);

        anythingChanged = true;

        if (use_debug_serial_display) {
          debugDisplay(x, y, col_char);
        }
      }
    }
  }

  //if any pixels were changed, updated all of the LEDs and store the current grid for comparison next frame
  if (anythingChanged) {
    pix0.show();
    pix1.show();
    pix2.show();
    pix3.show();
    pix4.show();

    //  //store the current grid for comparison
    for (int y = 0; y < NUM_ROWS; y++) {
      for (int x = 0; x < NUM_COLS; x++) {
        last_sent_grid[x][y] = pixel[x][y];
      }
    }
  }
}


void debugDisplay(int x, int y, char col) {
  String line = String(x) + "," + String(y) + "," + pixel[x][y] + "\n";
  debug_display_buffer += line;
  num_queued_debug_display++;

  if (num_queued_debug_display == 8) {
    sendDebugDisplayMessage();
  }

  //Serial.print(line);
}

void sendDebugDisplayMessage() {
  //send it
  Serial.print(debug_display_buffer);

  //reset it
  num_queued_debug_display = 0;
  debug_display_buffer = "";
}


//sending serial to Processing
//this does not really work
//void debugDisplay() {
//  //int start_time = millis();
//
//  String message = "";
//  //get the difference between current grid and last sent grid
//  for (int y = 0; y < NUM_ROWS; y++) {
//    for (int x = 0; x < NUM_COLS; x++) {
//      message += pixel[x][y];
//    }
//    message += '\n';
//  }
//  message += '|';
//
//  Serial.print(message);
//
//  //int end_time = millis() - start_time;
//  //String time_message = "it took "+String(end_time);
//  //Serial.println(time_message);
//}


//Was tyring to setup the debug display the same way that we were sending info from Processing during the jam but it was way too slow

//void debugDisplay() {
//  //int start_time = millis();
//
//  String message = "";
//  //get the difference between current grid and last sent grid
//  for (int y = 0; y < NUM_ROWS; y++) {
//    for (int x = 0; x < NUM_COLS; x++) {
//      if (pixel[x][y] != last_sent_grid[x][y]) {
//        String line = String(x) + "," + String(y) + "," + pixel[x][y] + "\n";
//        message += line;
//        //Serial.print(line);
//      }
//    }
//  }
//
//  //send the whole thing
//  if (message.length() > 2) {
//    Serial.print(message);
//  }
//
//  //store the current grid for comparison
//  for (int y = 0; y < NUM_ROWS; y++) {
//    for (int x = 0; x < NUM_COLS; x++) {
//      last_sent_grid[x][y] = pixel[x][y];
//    }
//  }
//
//  //int end_time = millis() - start_time;
//  //String time_message = "it took "+String(end_time);
//  //Serial.println(time_message);
//}
