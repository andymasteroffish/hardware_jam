#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

#define NUM_COLS 96
#define NUM_ROWS 5

#define NUM_OBSTACLES 8
#define NUM_PLAYERS 2

#define LETTER_WIDTH 5

boolean use_debug_serial_display = false;
boolean debug_skip_intro = false;
boolean debug_no_death = false;

//game values
int startSpeed = 230;  //measured in millis between steps
float speedMult = 0.75;  //the closer this is to 0, the faster they'll go
float slowDownMult = 1.5; //if we have one of the obstacles slow the players down, this is how much it will slow them

//speeding up over time
int millis_between_speed_up = 1000;
int next_speed_up_time = 0;
int speed_up_ammount = 2;//5;

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
  unsigned char r;
  unsigned char g;
  unsigned char b;

  //death animation
  boolean doingDeathAnim;
  int deathAnimStep;
  int nextDeathAnimStepTime;
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
int playerStarts[NUM_PLAYERS];

int winner = -1;
//boolean gameOver = false;
//boolean gameBegun = false;
#define STATE_INTRO 0
#define STATE_PREGAME 1
#define STATE_GAME 2
#define STATE_GAMEOVER 3
int gameState = STATE_INTRO;

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
#define NUM_BUTTONS 8
Button buttons[NUM_BUTTONS];

int button_lock_timer = 0;
int button_lock_time = 1000;

//LEDs
//this is ugly, but putting them in an array from the start keeps failing
//data pin then clock pin
//data is green, clock is yellow
Adafruit_DotStar pix0 = Adafruit_DotStar(NUM_COLS, 4, 5 , DOTSTAR_BRG);
Adafruit_DotStar pix1 = Adafruit_DotStar(NUM_COLS, 6, 7, DOTSTAR_BRG);
Adafruit_DotStar pix2 = Adafruit_DotStar(NUM_COLS+1, 11, 12, DOTSTAR_BRG);
Adafruit_DotStar pix3 = Adafruit_DotStar(NUM_COLS+1, 13, 14, DOTSTAR_BRG);    //right now this strip is messed up and we skip the first LED
Adafruit_DotStar pix4 = Adafruit_DotStar(NUM_COLS+1 , 15, 16, DOTSTAR_BRG); //there was an extra LED on this one

//trakcing which columns have changed this frame
int updatedCols[10];
int numUpdatedCols = 0;

void setup() {
  Serial.begin(345600);
  if (!use_debug_serial_display) {
    Serial.write("hello");
  }

  randomSeed(analogRead(0));

  gameState = STATE_INTRO;
  winner = -1;

  //default states
  resetMatrix();

  //setup players
  players[0].identifier = 0;
  players[0].r = 0;
  players[0].g = 0;
  players[0].b = 136;

  players[1].identifier = 10;
  players[1].r = 136;
  players[1].g = 136;
  players[1].b = 0;

  playerStarts[0] = 3;
  playerStarts[1] = 50;

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
  obstacles[0].action = 'a';
  obstacles[1].action = 'b';//'s';
  obstacles[2].action = 's';//'a';
  obstacles[3].action = 'b';//'s';
  obstacles[4].action = 'a';
  obstacles[5].action = 'b';
  obstacles[6].action = 'r';//'a';
  obstacles[7].action = 'b';//'s';

  //adjusting them into place
  obstacles[0].x = 9;
  obstacles[1].x += 10;
  obstacles[2].x += 8;
  obstacles[3].x += 10;
  obstacles[4].x += 9;
  obstacles[5].x += 10;
  obstacles[6].x += 8;
  obstacles[7].x += 10;

  //dpending on the type, turn a few on
//  for (int i = 0; i < NUM_OBSTACLES; i++) {
//    if (obstacles[i].action == 's') {
//      obstacles[i].onRows[0] = true;
//      obstacles[i].onRows[1] = true;
//    }
//    if (obstacles[i].action == 'b') {
//      obstacles[i].onRows[0] = true;
//      obstacles[i].onRows[1] = true;
//    }
//    if (obstacles[i].action == 'a') {
//      obstacles[i].onRows[0] = true;
//      obstacles[i].onRows[1] = true;
//    }
//    if (obstacles[i].action == 'r') {
//      obstacles[i].onRows[0] = true;
//      obstacles[i].onRows[1] = true;
//    }
//  }

  //buttons
  buttons[0].pin = 3;
  buttons[0].key = '0';

  buttons[1].pin = 17;
  buttons[1].key = '1';

  buttons[2].pin = 18;
  buttons[2].key = '2';

  buttons[3].pin = 19;
  buttons[3].key = '3';

  buttons[4].pin = 20;
  buttons[4].key = '4';

  buttons[5].pin = 0;
  buttons[5].key = '5';

  buttons[6].pin = 1;
  buttons[6].key = '6';

  buttons[7].pin = 2;
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

  if (debug_skip_intro){
    reset();  
    gameState = STATE_GAME;
  }
}

void reset() {
  if (!use_debug_serial_display) {
    Serial.println("game start");
  }

  //randomize obstacles (odd numbers are always blockers)
  for (int i=0; i<NUM_OBSTACLES; i+=2){
//    String this_massage = "set obstacle" + String(i);
//    Serial.print(this_massage);
    int rand_val = (int)random(0, 3);
    if (rand_val == 0) obstacles[i].action = 'a';
    if (rand_val == 1) obstacles[i].action = 's';
    if (rand_val == 2) obstacles[i].action = 'r';
  }
  //make sure we have at leats one accelerator
  int rand_obstacle = (int)random(0, 3) * 2;
//  String another_massage = "set obstacle" + String(rand_obstacle);
//  Serial.print(another_massage);
  obstacles[rand_obstacle].action = 'a';

  String ("done setting obstacles");
  
  //set them up
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    for (int k=0; k<5; k++){
      obstacles[i].onRows[k] = false;
    }
    if (obstacles[i].action == 's') {
      obstacles[i].onRows[0] = true;
      obstacles[i].onRows[1] = true;
    }
    if (obstacles[i].action == 'b') {
      obstacles[i].onRows[0] = true;
      obstacles[i].onRows[1] = true;
    }
    if (obstacles[i].action == 'a') {
      obstacles[i].onRows[0] = true;
      obstacles[i].onRows[1] = true;
    }
    if (obstacles[i].action == 'r') {
      obstacles[i].onRows[0] = true;
      obstacles[i].onRows[1] = true;
    }
  }

  gameState = STATE_PREGAME;

  for (int i = 0; i < NUM_PLAYERS; i++) {
    players[i].x = playerStarts[i];
    //if (NUM_PLAYERS == 2) players[i].y = i * (NUM_ROWS - 1); //put on opposite side
    //else                  players[i].y = i; //use its own lane
    players[0].y = 1;
    players[1].y = 3;
    players[i].speed = startSpeed;
    players[i].dir = 1;

    players[i].doingDeathAnim = false;
    players[i].deathAnimStep = 0;
  }


  //run through obstacles and randomize them
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    int presses = (int)random(0, NUM_ROWS);
    for (int k = 0; k < presses; k++) {
      shiftObstacle(i);
    }
  }

  next_speed_up_time = millis();

}

void loop() {
  checkInput();

  if (gameState == STATE_INTRO)          displayIntro();
  else if (gameState == STATE_PREGAME)   displayPregame();
  else if (gameState == STATE_GAME)      runGame();
  else if (gameState == STATE_GAMEOVER)  displayWinner(winner);

  //displaying the thing
  setLEDs();

  if (use_debug_serial_display && num_queued_debug_display > 0) {
    sendDebugDisplayMessage();
  }

}

void runGame() {

  winner = checkWinners(); //winner remains -1 if no winner
  if (winner != -1 && gameState == STATE_GAME && !checkDeathAnimations()) {
    gameState = STATE_GAMEOVER;
    //if the game just ended, lock the buttons for a bit
    button_lock_timer = millis() + button_lock_time;
    if (use_debug_serial_display) {
      Serial.println("game just ended");
    }
  }

  //update our players
  for (int i = 0; i < NUM_PLAYERS; i++) {
    //is it time to move?
    if (millis() > players[i].nextMoveTime && players[i].speed > 0 && !checkDeathAnimations()) {
      advancePlayer(i);
      players[i].nextMoveTime = millis() + players[i].speed;
    }
  }

  //time to speed up?
  if (millis() > next_speed_up_time && !checkDeathAnimations()){
    next_speed_up_time = millis() + millis_between_speed_up;
    for (int i = 0; i < NUM_PLAYERS; i++) {
      if (players[i].speed > 50){
        players[i].speed -= speed_up_ammount;
      }
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
    players[p].x = NUM_COLS - 1;
  }

  //check for obstacles
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    if (obstacles[i].x == players[p].x) {
      boolean hitMe = false;
      for (int r = 0; r < NUM_ROWS; r++) {
        if (obstacles[i].onRows[r] && r == players[p].y) {
          //dis fool hit duh obsacle lolz
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
  if (action == 'b' && players[p].speed != 0 && !checkDeathAnimations() && !debug_no_death) {
    killPlayer(p);
  }

  //accelerate (or slow down????)
  if (action == 'a') {
    players[p].speed *= speedMult;
    //trying out slowing the player down!
    //players[p].speed *= slowDownMult;
    //keep it positive
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

void killPlayer(int id) {
  players[id].speed = 0;
  players[id].doingDeathAnim = true;
  players[id].nextDeathAnimStepTime = millis() + deathAnimStepTime;
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

boolean checkDeathAnimations() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (players[i].doingDeathAnim == true) {
      return true;
    }
  }
  return false;
}

void checkInput() {
  //check if controls are locked out right now
  if (millis() < button_lock_timer) {
    //Serial.println("lock out");
    return;
  }

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
//    Serial.print("pressed ");
//    Serial.println(id);
    Serial.print("button pin ");
    Serial.println(buttons[id].pin);
    //return; //kill me
  }
  //reset the game if we're not playing
  if (gameState == STATE_INTRO) {
    reset();
    pregameStep = 0;
    nextPregameStepTime = millis();
  } else if (gameState == STATE_GAMEOVER) {
    gameState = STATE_INTRO;
    button_lock_timer = millis() + button_lock_time;
  }

  //bump this obstacle down one
  shiftObstacle(id);
}

void shiftObstacle(int id) {
  boolean temp = obstacles[id].onRows[NUM_ROWS - 1];
  for (int r = NUM_ROWS - 1; r > 0; r--) {
    obstacles[id].onRows[r] = obstacles[id].onRows[r - 1];
  }
  obstacles[id].onRows[0] = temp;
}

void displayGame() {
  resetMatrix();
  //Serial.println("doing display");
  //do the player trails before the obstacles
  for (int i = 0; i < NUM_PLAYERS; i++) {
//      Serial.println("trails for player:");
//      Serial.println(i);

      //getting the trial positions here to be sure that they loop correctly
      //previously these were causing index out of bounds errors
      int trail_x_1 = (players[i].x - players[i].dir     + NUM_COLS) % NUM_COLS;
      int trail_x_2 = (players[i].x - players[i].dir * 2 + NUM_COLS) % NUM_COLS;
      int trail_x_3 = (players[i].x - players[i].dir * 3 + NUM_COLS) % NUM_COLS;

    if (!players[i].doingDeathAnim) {
      //Serial.println("trails");
      //Trails
      //im not sure what the speed range is like, so just doing generic trails manually for now
      //also wont show right if it shifts, but we could store the previous positions if it feels weird
      pixel[trail_x_1][players[i].y] = players[i].identifier + 4;
      pixel[trail_x_2][players[i].y] = players[i].identifier + 6;
      pixel[trail_x_3][players[i].y] = players[i].identifier + 9;
    } else {
      //Serial.println("dead trails");
      pixel[trail_x_1][players[i].y] = '-';
      pixel[trail_x_2][players[i].y] = '-';
      pixel[trail_x_3][players[i].y] = '-';
    }

    //add the obstacles
    for (int i = 0; i < NUM_OBSTACLES; i++) {
      for (int r = 0; r < NUM_ROWS; r++) {
        if (obstacles[i].onRows[r]) {
          pixel [obstacles[i].x][r] = obstacles[i].action;
        }
      }
    }

    //add the players over them
    for (int i = 0; i < NUM_PLAYERS; i++) {
      //sometime splayer x position is garbage data
      //this is a sloppy solution, but it was causing crashes and I need to go home soon
      if (players[i].x < 0 || players[i].x >= NUM_COLS){
        players[i].x = playerStarts[i];
      }
      
//      Serial.println("this player x");
//      Serial.println(players[i].x);
      if (!players[i].doingDeathAnim)   pixel[players[i].x][players[i].y] = players[i].identifier; //full power
      else pixel[players[i].x][players[i].y] = '-';
    }

    //death animations
    for (int i = 0; i < NUM_PLAYERS; i++) {
      if (players[i].doingDeathAnim == true) {
        //how far to go
        int dist = 1 + players[i].deathAnimStep;

        //get the points
        int x_left = (players[i].x - dist + NUM_COLS) % NUM_COLS; //these need to loop
        int x_right = (players[i].x + dist) % NUM_COLS;           //these need to loop
        int y_top = players[i].y - dist + players[i].deathAnimStep; //adding deathanimstep is like gravity? might be too fast
        int y_bot = players[i].y + dist + players[i].deathAnimStep;
        int y_mid = players[i].y + players[i].deathAnimStep;

        //add them to the pixels (last few frames are blank)
        if (players[i].deathAnimStep < 3) {
          //diagonal
          if (y_top >= 0)        pixel[x_left][y_top] = players[i].identifier + players[i].deathAnimStep; //add the step # so player death particles become 10% weaker each step
          if (y_bot < NUM_ROWS)  pixel[x_left][y_bot] = players[i].identifier + players[i].deathAnimStep;
          if (y_top >= 0)        pixel[x_right][y_top] = players[i].identifier + players[i].deathAnimStep;
          if (y_bot < NUM_ROWS)  pixel[x_right][y_bot] = players[i].identifier + players[i].deathAnimStep;

          //horizontal
          if (y_mid >= 0 && y_mid < NUM_ROWS) {
            pixel[x_left][players[i].y] = players[i].identifier + players[i].deathAnimStep;
            pixel[x_right][players[i].y] = players[i].identifier + players[i].deathAnimStep;
          }
        }

        //time to update?
        if (players[i].nextDeathAnimStepTime < millis()) {
          players[i].deathAnimStep++;
          players[i].nextDeathAnimStepTime = millis() + deathAnimStepTime;

          if (players[i].deathAnimStep == 5) {
            players[i].doingDeathAnim = false;
          }
        }
      }
    }
  }
}

  void displayIntro() {
    int mod = (millis() / 400) % NUM_COLS;
    String abc = "-bsar01";
    //println("mod=" + mod);

    //normal display
    if (!use_debug_serial_display) {

      //flahsing
      for (int y = 0; y < NUM_ROWS; y++) {
        for (int x = 0; x < NUM_COLS; x++) {
          int loc = x + mod + y * NUM_COLS;
          pixel[x][y] = abc.charAt(loc % abc.length());
        }
      }

      //show the title
      String title_text = "circumnavigators";
      int title_x = NUM_COLS - (millis() / 70) % (title_text.length() * (LETTER_WIDTH + 1) + NUM_COLS);
      printWord(title_text, 'r', title_x);
    }

    //debug display with fewer changing pixels
    else {
      for (int y = 0; y < NUM_ROWS; y++) {
        pixel[mod][y] = 'b';
      }
    }
  }

  void displayPregame() {
    //time to advance
    if (millis() > nextPregameStepTime) {
      nextPregameStepTime = millis() + 50;
      pregameStep++;
    }

    //blank the board
    resetMatrix();

    //show the characters coming in with arrows
    int trackPos = (NUM_COLS / 2) - pregameStep;
    if (trackPos < 0)  trackPos = 0;
    for (int p = 0; p < NUM_PLAYERS; p++) {
      for (int i = 0; i < trackPos; i++) {
        int x1 = (playerStarts[p] + i) % NUM_COLS;
        int x2 = (playerStarts[p] - i + NUM_COLS) % NUM_COLS;

        if (i != trackPos - 3) {
          pixel[x1][players[p].y] = players[p].identifier;
          pixel[x2][players[p].y] = players[p].identifier;
        }

        //arrow tail
        if (i == trackPos - 1) {
          int x1Shift = (x1 + 1) % NUM_COLS;
          int x2Shift = (x2 - 1 + NUM_COLS) % NUM_COLS;
          pixel[x1][players[p].y - 1] = players[p].identifier;
          pixel[x1Shift][players[p].y - 1] = players[p].identifier;
          pixel[x1Shift][players[p].y + 1] = players[p].identifier;
          pixel[x1][players[p].y + 1] = players[p].identifier;

          pixel[x2][players[p].y - 1] = players[p].identifier;
          pixel[x2Shift][players[p].y - 1] = players[p].identifier;
          pixel[x2Shift][players[p].y + 1] = players[p].identifier;
          pixel[x2][players[p].y + 1] = players[p].identifier;
        }
      }
    }

    //blink the game
    //Serial.println("blink");
    if (pregameStep > NUM_COLS / 2) {
      if (pregameStep % 4 < 2) {
        //Serial.println("display");
        displayGame();
      }

      //make sure the players are shown
      for (int i = 0; i < NUM_PLAYERS; i++) {
        pixel[players[i].x][players[i].y] = players[i].identifier;
      }
    }

    //after a bit, go to the game
    if (pregameStep == NUM_COLS / 2 + 20) {
      Serial.println("go to game");
      gameState = STATE_GAME;
    }
  }

  void displayWinner(int player) {
    int mod = (millis() / 50) % NUM_COLS;
    //println("PLAYER " + player + " WON!!!" + " mod=" + mod);
    for (int y = 0; y < NUM_ROWS; y++) {
      for (int x = 0; x < NUM_COLS; x++) {
        int loc = x + y * NUM_COLS;
        pixel[x][y] = players[player].identifier;// Integer.toString(player).charAt(0);
        if (loc % NUM_COLS == mod || (loc + NUM_COLS / 3) % NUM_COLS == mod || (loc + (NUM_COLS / 3) * 2) % NUM_COLS == mod ) pixel[x][y] = '-';
      }
    }

     String title_text = "winner";
      int title_x = NUM_COLS - (millis() / 100) % (title_text.length() * (LETTER_WIDTH + 1) + NUM_COLS);
      printWord(title_text,  players[player].identifier, title_x);
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
          uint32_t color = 0x000000;  //'-' (ascii 45)

          if (col_char == 'b') color = 0x004400;  //blocked (ascii 98)
          if (col_char == 's') color = 0x444444;  //shifter (ascii 115)
          if (col_char == 'a') color = 0x440000;  //accelerator (ascii 97)
          if (col_char == 'r') color = 0x004444;  //reverse (ascii 114)

          if (col_char / 10 == 0) { //p1
            float pct = (10.0 - col_char % 10) / 10.0;
            if (pct != 1.0) pct /= 10; //lowering the trails to 1/10 power
            color = pix0.Color(players[0].r * pct, players[0].g * pct, players[0].b * pct); //just using the dotstar class to store the color
          }
          if (col_char / 10 == 1) { //p2
            float pct = (10.0 - col_char % 10) / 10.0;
            if (pct != 1.0) pct /= 10; //lowering the trails to 1/10 power
            color = pix0.Color(players[1].r * pct, players[1].g * pct, players[1].b * pct);
          }

          //if (col_char / 10 == 2) { //p3
          //   float pct = (10 - col_char % 10) / 10;
          //   color = pix0.Color(players[2].r * pct, players[2].g * pct, players[2].b * pct);
          //}

          //if (col_char == '0') color = 0x000088;  //p1
          //if (col_char == '1') color = 0x888800;  //p2
          //if (col_char == '2') color = 0x000044;  //p3


          if (y == 0) pix0.setPixelColor(NUM_COLS - 1 - x , color);
          if (y == 1) pix1.setPixelColor(NUM_COLS - 1 - x -1, color);
          if (y == 2) pix2.setPixelColor(NUM_COLS - 1 - x , color);
          if (y == 3) pix3.setPixelColor(NUM_COLS - 1 - x , color);
          if (y == 4) pix4.setPixelColor(NUM_COLS - 1 - x , color);

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
