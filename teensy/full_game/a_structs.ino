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
  int pX[MAX_TRAILS];

  int dist_traveled;

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

  uint32_t col;
  int led_id;
};
