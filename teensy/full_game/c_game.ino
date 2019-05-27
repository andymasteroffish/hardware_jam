

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
  //blue
  players[0].identifier = 0;
  players[0].col.set(0,0,136);
  playerStarts[0] = 8;
  join_areas[0].setup(8, &players[0].col);

  //yellow
  players[1].identifier = 10;
  players[1].col.set(136,136,0);
  playerStarts[1] = 22;
  join_areas[1].setup(22, &players[1].col);

  //orange
  players[2].identifier = 20;
  players[2].col.set(200,50,0);
  playerStarts[2] = 31;
  join_areas[2].setup(31, &players[2].col);
  

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
  obstacles[0].action = 'b';
  obstacles[1].action = 'b';
  obstacles[2].action = 'b';
  obstacles[3].action = 'b';
  obstacles[4].action = 'b';
  obstacles[5].action = 'b';
  obstacles[6].action = 'b';
  obstacles[7].action = 'b';

  //adjusting them into place
  obstacles[0].x = 9;
  obstacles[1].x += 10;
  obstacles[2].x += 8;
  obstacles[3].x += 10;
  obstacles[4].x += 9;
  obstacles[5].x += 10;
  obstacles[6].x += 8;
  obstacles[7].x += 10;

  //just move them allover a bit
  for (int i = 0; i < NUM_OBSTACLES; i ++) {
    obstacles[i].x-= 1;
  }

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
    buttons[i].led_id = (i+3) % NUM_BUTTONS;
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  //LEDs
  for (int i=0; i<NUM_ROWS; i++){
    leds[i].begin();
    leds[i].show();
  }
//  pix0.begin();
//  pix0.show();
//
//  pix1.begin();
//  pix1.show();
//
//  pix2.begin();
//  pix2.show();
//
//  pix3.begin();
//  pix3.show();
//
//  pix4.begin();
//  pix4.show();

  //buttons
  button_pixels.begin();

  delta_millis = 0;
  prev_frame_millis = millis();

  //some default colors
  int col_val = 100;
  blank_col.set(0,0,0);
  blocker_col.set(col_val,0,0);
  shifter_col.set(col_val,col_val,col_val);
  accelerator_col.set(0,col_val,0);
  reverse_col.set(col_val,0,col_val);
  
  
  //go right to the game if we're tetsing
  if (debug_skip_intro) {
    reset();
    gameState = STATE_GAME;
  }
}

void reset() {
  if (!use_debug_serial_display) {
    Serial.println("game start");
  }

  //randomize obstacles (every other is blocker)
  for (int i = 0; i < NUM_OBSTACLES; i ++) {
    //    String this_massage = "set obstacle" + String(i);
    //    Serial.print(this_massage);
    if (i%2==1){
      obstacles[i].action = 'b';
    }else{
      int rand_val = (int)random(0, 3);
      if (rand_val == 0) obstacles[i].action = 'a';
      if (rand_val == 1) obstacles[i].action = 's';
      if (rand_val == 2) obstacles[i].action = 'r';
    }
  }
  //make sure we have at leats one accelerator
  int rand_obstacle = (int)random(0, 3) * 2;
  //  String another_massage = "set obstacle" + String(rand_obstacle);
  //  Serial.print(another_massage);
  obstacles[rand_obstacle].action = 'a';


  String ("done setting obstacles");

  //set them up
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    for (int k = 0; k < 5; k++) {
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

  if (num_players == 2){
    players[0].y = 1;
    players[1].y = 3;
    players[2].y = 0;
  }
  if (num_players == 3){
    players[0].y = 1;
    players[1].y = 2;
    players[2].y = 3;
  }

  //have the join areas do their thing
  for (int i=0; i<num_players; i++){
    join_areas[i].mark_game_start(players[i].y);
  }

  for (int i = 0; i < MAX_NUM_PLAYERS; i++) {
    players[i].x = playerStarts[i];
    for (int j = 0; j < MAX_TRAILS; j++) players[i].pX[j] = players[i].x;
    //if (num_players == 2) players[i].y = i * (NUM_ROWS - 1); //put on opposite side
    //else                  players[i].y = i; //use its own lane
    players[i].speed = startSpeed;
    players[i].dir = 1;
    players[i].dist_traveled = 0;

    players[i].doingDeathAnim = false;
    players[i].deathAnimStep = 0;

    //if this player is out, mark them as dead
    if (i >= num_players){
      players[i].speed = 0;
    }
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

void startJoinScreen(){
  gameState = STATE_JOIN;
  join_sreen_start_time = millis();
  join_screen_end_timer = 1500;
  for (int i=0; i<MAX_NUM_PLAYERS; i++){
    join_areas[i].reset();
  }
  resetMatrix();
}

void loop() {
  delta_millis = millis() - prev_frame_millis;
  prev_frame_millis = millis();
  checkInput();

  //Serial.println(gameState);
  if (gameState == STATE_INTRO)          displayIntro();
  else if (gameState == STATE_JOIN)      displayJoin();
  else if (gameState == STATE_PREGAME)   displayPregame();
  else if (gameState == STATE_GAME)      runGame();
  else if (gameState == STATE_GAMEOVER)  displayWinner(winner);
  else if (gameState == STATE_SETTINGS)  displaySettings();

  //displaying the thing
  setLEDs();

  if (use_debug_serial_display && num_queued_debug_display > 0) {
    sendDebugDisplayMessage();
  }

  delay(10);

}

void runGame() {

  int live_players = 0;
  for (int i = 0; i < num_players; i++) {
    if (players[i].speed > 0){
      live_players++; 
    }
  }
  //Serial.println(live_players);

  winner = checkWinners(); //winner remains -1 if no winner
  if (winner != -1 && gameState == STATE_GAME && !checkDeathAnimations()) {
    gameState = STATE_GAMEOVER;
    game_over_time = millis();
    end_game_over_time = millis() + max_game_over_time;
    //if the game just ended, lock the buttons for a bit
    button_lock_timer = millis() + button_lock_time;
    if (use_debug_serial_display) {
      Serial.println("game just ended");
    }
  }

  //update our players
  for (int i = 0; i < num_players; i++) {
    //is it time to move?
    if (millis() > players[i].nextMoveTime && players[i].speed > 0 && (live_players > 1 || !checkDeathAnimations()) ) {
      advancePlayer(i);
      players[i].nextMoveTime = millis() + players[i].speed;
    }
  }

  //time to speed up?
  if (millis() > next_speed_up_time && !checkDeathAnimations()) {
    next_speed_up_time = millis() + millis_between_speed_up;
    for (int i = 0; i < num_players; i++) {
      if (players[i].speed > 50) {
        players[i].speed -= speed_up_ammount;
      }
    }
  }

  //render
  displayGame();
}

void advancePlayer(int p) {
  players[p].dist_traveled++;
  //store the old positions
  for (int k = MAX_TRAILS - 1; k > 1; k--)
    players[p].pX[k] = players[p].pX[k - 1];

  players[p].pX[0] = players[p].x;

  //move
  players[p].x += players[p].dir;
  if (players[p].x == NUM_COLS) {
    players[p].x = 0;
  }
  if (players[p].x == -1) {
    players[p].x = NUM_COLS - 1;
  }

  //check for obstacles
  //if (players[0].dist_traveled > NUM_COLS+2){     //no getting hit on your first time around
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
  //}
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
  for (int i = 0; i < num_players; i++) {
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
  for (int i = 0; i < num_players; i++) {
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
          if (gameState == STATE_SETTINGS){
            button_pressed_settings(i);
          }else{
            button_pressed(i);
          }
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
    startJoinScreen();
  }else if (gameState == STATE_JOIN){
    if (id < num_players){
      join_areas[id].mark_ready();
    }
  }else if (gameState == STATE_GAMEOVER) {
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
  for (int i = 0; i < num_players; i++) {


    //getting the trial positions here to be sure that they loop correctly
    //previously these were causing index out of bounds errors
    //Updated: hardcoded trails for time ... fix later to make dynamic
    int trail_x_1 = (players[i].x - players[i].dir     + NUM_COLS) % NUM_COLS;
    int trail_x_2 = (players[i].x - players[i].dir * 2 + NUM_COLS) % NUM_COLS;
    int trail_x_3 = (players[i].x - players[i].dir * 3 + NUM_COLS) % NUM_COLS;
    int trail_x_4 = (players[i].x - players[i].dir * 4 + NUM_COLS) % NUM_COLS;

    if (!players[i].doingDeathAnim && players[i].speed != 0) {
      //Serial.println("trails");
      //Trails
      //im not sure what the speed range is like, so just doing generic trails manually for now
      //also wont show right if it shifts, but we could store the previous positions if it feels weird

      //starter trail
      if (players[i].speed > startSpeed * speedMult) {
        float power = 0.02f;
        pixel[trail_x_1][players[i].y].set(&players[i].col, power);
        //players[i].identifier + 8;
      } else if (players[i].speed <= startSpeed * speedMult) {
        float power = 0.1f;
        pixel[trail_x_1][players[i].y].set(&players[i].col, power);
        power = 0.02f;
        pixel[trail_x_2][players[i].y].set(&players[i].col, power);
        //power = 0.03f;
        //pixel[trail_x_3][players[i].y].set(players[i].col, power);
      }
    } else {
      //Serial.println("dead trails");
      pixel[trail_x_1][players[i].y].blank();
      pixel[trail_x_2][players[i].y].blank();
      pixel[trail_x_3][players[i].y].blank();
    }

    //add the obstacles
    for (int i = 0; i < NUM_OBSTACLES; i++) {
      //get the color
      ColorHolder * color = &blank_col;
      if (obstacles[i].action == 'b') color = &blocker_col;//.r*global_brightness,   blocker_col.g*global_brightness, blocker_col.b*global_brightness);
      if (obstacles[i].action == 's') color = &shifter_col;//.r*global_brightness, shifter_col.g*global_brightness, shifter_col.b*global_brightness);
      if (obstacles[i].action == 'a') color = &accelerator_col;//.r*global_brightness, accelerator_col.g*global_brightness,   accelerator_col.b*global_brightness);
      if (obstacles[i].action == 'r') color = &reverse_col;//.r*global_brightness, reverse_col.g*global_brightness, reverse_col.b*global_brightness);

      buttons[i].col.set(color);
      
      for (int r = 0; r < NUM_ROWS; r++) {
        if (obstacles[i].onRows[r]) {
          pixel[obstacles[i].x][r].set(color);
        }
      }
    }

    //add the players over them
    for (int i = 0; i < num_players; i++) {
      if(players[i].speed > 0){
        //sometimes player x position is garbage data
        //this is a sloppy solution, but it was causing crashes and I need to go home soon
        if (players[i].x < 0 || players[i].x >= NUM_COLS) {
          players[i].x = playerStarts[i];
        }
  
        if (!players[i].doingDeathAnim && players[i].speed != 0) {
          pixel[players[i].x][players[i].y].set(&players[i].col);// = pix0.Color( players[i].r,  players[i].g,  players[i].b);// players[i].identifier; //full power
        }
        else {
          pixel[players[i].x][players[i].y].blank();
        }
      }
    }

    //death animations
    
    for (int i = 0; i < num_players; i++) {
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
          float power = 0.1 - (float)players[i].deathAnimStep * 0.02;
          if (y_top >= 0)        pixel[x_left][y_top].set(&players[i].col, power);// = pix0.Color( players[i].r*power*global_brightness,  players[i].g*power*global_brightness,  players[i].b*power*global_brightness);// players[i].identifier + players[i].deathAnimStep; //add the step # so player death particles become 10% weaker each step
          if (y_bot < NUM_ROWS)  pixel[x_left][y_bot].set(&players[i].col, power);// = pix0.Color( players[i].r*power*global_brightness,  players[i].g*power*global_brightness,  players[i].b*power*global_brightness);//players[i].identifier + players[i].deathAnimStep;
          if (y_top >= 0)        pixel[x_right][y_top].set(&players[i].col, power);// = pix0.Color( players[i].r*power*global_brightness,  players[i].g*power*global_brightness,  players[i].b*power*global_brightness);//players[i].identifier + players[i].deathAnimStep;
          if (y_bot < NUM_ROWS)  pixel[x_right][y_bot].set(&players[i].col, power);// = pix0.Color( players[i].r*power*global_brightness,  players[i].g*power*global_brightness,  players[i].b*power*global_brightness);//players[i].identifier + players[i].deathAnimStep;

          //horizontal
          if (y_mid >= 0 && y_mid < NUM_ROWS) {
            pixel[x_left][players[i].y].set(&players[i].col, power);// pix0.Color( players[i].r*power*global_brightness,  players[i].g*power*global_brightness,  players[i].b*power*global_brightness);//players[i].identifier + players[i].deathAnimStep;
            pixel[x_right][players[i].y].set(&players[i].col, power);// = pix0.Color( players[i].r*power*global_brightness,  players[i].g*power*global_brightness,  players[i].b*power*global_brightness);//players[i].identifier + players[i].deathAnimStep;
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
  //Serial.println("mod=" + mod);

  //flashing
  for (int y = 0; y < NUM_ROWS; y++) {
    for (int x = 0; x < NUM_COLS; x++) {
      int loc = x + mod + y * NUM_COLS;
      char this_char = abc.charAt(loc % abc.length());
      ColorHolder * col = &blank_col;
      if (this_char == 'b') col = &blocker_col;
      if (this_char == 's') col = &shifter_col;
      if (this_char == 'a') col = &accelerator_col;
      if (this_char == 'r') col = &reverse_col;
      if (this_char == '0') col = &players[0].col;
      if (this_char == '1') col = &players[1].col;

      pixel[x][y].set(col);
    }
  }

  //show the title
  String title_text = "circumnavigators";
  int title_x = NUM_COLS - (millis() / 70) % (title_text.length() * (LETTER_WIDTH + 1) + NUM_COLS);
  printWord(title_text, &reverse_col, title_x);
  

  //messing with the buttons
//  int light_offset = (millis()/100) % NUM_BUTTONS;
//
  float time_val = ((float)millis()/1000.0) * 4;
  float anim_time = time_val;
  float anim_range = 2.0f;
  
  int steps = NUM_BUTTONS+4;
  while(anim_time > (float)steps){
    anim_time -= (float)steps;
  }
  anim_time -= 2;
  

  //Serial.println(anim_time);
 
  for (int i=0; i<NUM_BUTTONS; i++){
    float r = 255;
    float g = 0;
    float b = 255;

    float prc = 0;

    
    float this_time = abs(anim_time-( (NUM_BUTTONS-1-i)+1));
    if (this_time < anim_range){
      prc = 1.0 - this_time / (float)anim_range;
    }

    buttons[i].col.set(reverse_col.r, reverse_col.g, reverse_col.b, prc); 
    
  }
}

void displayJoin(){
  
  resetMatrix();
  
  float time_on_screen = (millis()-join_sreen_start_time)/1000.0;

  //pulse the buttons
  for (int i=0; i<num_players; i++){
    
    float prc = 0.5 +sin(time_on_screen*10) * 0.5;

    if (join_areas[i].player_joined){
      prc = 1;
    }

    buttons[i].col.set(&players[i].col, prc);// = button_pixels.Color(r,g,b);
  }
  //blank other buttons
  for (int i=num_players; i<NUM_BUTTONS; i++){
     buttons[i].col.blank();
  }

  
  //show who's there
  int col_w = 5;
  for (int i=0; i<num_players; i++){
    join_areas[i].update(delta_millis, pixel);
  }

  //time to move on?
  int num_in = 0;
  for (int i=0; i<num_players; i++){
    if (join_areas[i].player_joined){
      num_in++;
    }
  }
  if (num_in == num_players){
    join_screen_end_timer -= delta_millis;
  }

  if (join_screen_end_timer < 0){
    reset();
    pregameStep = 0;
    nextPregameStepTime = millis();
  }

  //gentle arrows to direct players
  if (num_in < num_players){
    int opposite_x = playerStarts[1] + NUM_COLS/2;
    int arrow_track_length = 29;
    int arrow_length = 6;
    int frame = (millis()/100) % (arrow_track_length+arrow_length+arrow_length) -arrow_length;
    //Serial.println(frame);
    
    int left_x = opposite_x + frame;
    int right_x = opposite_x - frame;

    int arrow_r = 0;
    int arrow_g = 255;
    int arrow_b = 0;
    float arrow_alpha = 0.02f;
    for (int x=left_x; x<left_x+arrow_length; x++){
       for (int y=0; y<NUM_ROWS; y++){
        bool fill_in = false;
        if (y==2){
          fill_in = true;
        }
        if ( (y==1 || y == 3) && x==left_x+4){
          fill_in = true;
        }
        if ( (y==0 || y == 4) && x==left_x+3){
          fill_in = true;
        }
        if (x > opposite_x + arrow_track_length || x < opposite_x){
          fill_in = false;
        }
        if (fill_in){
          pixel[x%NUM_COLS][y].set(arrow_r, arrow_g, arrow_b, arrow_alpha);
        }
       }
    }
    for (int x=right_x; x>right_x-arrow_length; x--){
       for (int y=0; y<NUM_ROWS; y++){
        bool fill_in = false;
        if (y==2){
          fill_in = true;
        }
        if ( (y==1 || y == 3) && x==right_x-4){
          fill_in = true;
        }
        if ( (y==0 || y == 4) && x==right_x-3){
          fill_in = true;
        }
        if (x < opposite_x - arrow_track_length || x > opposite_x){
          fill_in = false;
        }
        if (fill_in){
          pixel[x%NUM_COLS][y].set(arrow_r, arrow_g, arrow_b, arrow_alpha);
        }
       }
    }
  }
  
}

void displayPregame() {
  
  //time to advance
  if (millis() > nextPregameStepTime) {
    nextPregameStepTime = millis() + 60;
    pregameStep++;
  }

  //check if the settings buttons are being held
  if(check_holding_for_settings()){
    Serial.println("go to settings");
    gameState = STATE_SETTINGS;
    return;
  }

  //blank the board
  resetMatrix();

  
  

  //show the characters coming in with arrows
  int trackPos = (NUM_COLS / 2) - pregameStep;
  if (trackPos < 0)  trackPos = 0;
  for (int p = 0; p < num_players; p++) {
    for (int i = 0; i < trackPos; i++) {
      int x1 = (playerStarts[p] + i) % NUM_COLS;
      int x2 = (playerStarts[p] - i + NUM_COLS) % NUM_COLS;

      if (i != trackPos - 3) {
        pixel[x1][players[p].y].set(&players[p].col);// = player_col;
        pixel[x2][players[p].y].set(&players[p].col);//;
      }

      //arrow tail
      if (i == trackPos - 1) {
        int x1Shift = (x1 + 1) % NUM_COLS;
        int x2Shift = (x2 - 1 + NUM_COLS) % NUM_COLS;
        pixel[x1][players[p].y - 1].set(&players[p].col);
        pixel[x1Shift][players[p].y - 1].set(&players[p].col);
        pixel[x1Shift][players[p].y + 1].set(&players[p].col);
        pixel[x1][players[p].y + 1].set(&players[p].col);

        pixel[x2][players[p].y - 1].set(&players[p].col);
        pixel[x2Shift][players[p].y - 1].set(&players[p].col);
        pixel[x2Shift][players[p].y + 1].set(&players[p].col);
        pixel[x2][players[p].y + 1].set(&players[p].col);
      }
    }
  }

  //show the join areas
  for (int i=0; i<num_players; i++){
    join_areas[i].update(delta_millis, pixel);
  }

  //blink the game
  if (pregameStep > NUM_COLS / 2) {
    if (pregameStep % 4 < 2) {
      displayGame();
    }

    //make sure the players are shown
    for (int i = 0; i < num_players; i++) {
      pixel[players[i].x][players[i].y].set(&players[i].col);
    }
  }

  //after a bit, go to the game
  if (pregameStep == NUM_COLS / 2 + 20) {
    Serial.println("go to game");
    gameState = STATE_GAME;
  }

}

//get to settings by holding several buttons down at once during the intro
bool check_holding_for_settings() {
  int num_held = 0;
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (digitalRead(buttons[i].pin) == LOW) {
        num_held++;
    }
  }

  if (num_held >= 4){
    settings_timer++;
    if (settings_timer >= settings_timer_to_trigger){
      settings_timer = 0;
      return true;
    }
  }

  return false;
}

void displayWinner(int player) {
  
  int mod = (millis() / 50) % NUM_COLS;
  //println("PLAYER " + player + " WON!!!" + " mod=" + mod);

  bool show_flash = millis() < game_over_time + 500;
  if (show_flash){
    for (int y = 0; y < NUM_ROWS; y++) {
      for (int x = 0; x < NUM_COLS; x++) {
        int loc = x + y * NUM_COLS;
        //uint32_t player_col = pix0.Color(players[player].r*global_brightness, players[player].g*global_brightness, players[player].b*global_brightness);
        pixel[x][y].set(&players[player].col);// = player_col;
        if (loc % NUM_COLS == mod || (loc + NUM_COLS / 3) % NUM_COLS == mod || (loc + (NUM_COLS / 3) * 2) % NUM_COLS == mod ) pixel[x][y].blank();
      }
    }
  }  
  else{
    String title_text = "winner winner";
    int title_x = NUM_COLS - (millis() / 100) %  NUM_COLS;
    //uint32_t player_col = pix0.Color(players[player].r*global_brightness, players[player].g*global_brightness, players[player].b*global_brightness);
    printWord(title_text,  &players[player].col, title_x, true);
  }

  if (millis() > end_game_over_time) {
    gameState = STATE_INTRO;
    button_lock_timer = millis() + button_lock_time;
  }

  //set the buttons
  for (int i=0; i<NUM_BUTTONS; i++){
    float prc = 0.5 + sin(millis()/100) * 0.5;
    buttons[i].col.set(&players[player].col, prc);
  }

}

void displaySettings(){
  resetMatrix();

  for (int i=0; i<NUM_BUTTONS; i++){
    buttons[i].col.blank();//set(&reverse_col);
  }
  

  //1 is brightness
  int brightness_bar_height = global_brightness_setting;
  for (int y = 0; y < NUM_ROWS; y++) {
    if (NUM_ROWS-y <= brightness_bar_height){
      pixel[obstacles[1].x][y].set(100,100,100);
    }
  }
  buttons[1].col.set(&shifter_col);

  //2 is num players
  String player_num_text = String(num_players);
  printWord(player_num_text, &shifter_col, obstacles[2].x-2);
  buttons[2].col.set(&shifter_col);

  //3 is start speed

  //4 is exit (drawing a check mark)
  int check_x = obstacles[4].x;
  pixel[check_x-2][2].set(&accelerator_col);// = 'a';
  pixel[check_x-1][3].set(&accelerator_col);// = 'a';
  pixel[check_x+0][2].set(&accelerator_col);// = 'a';
  pixel[check_x+1][1].set(&accelerator_col);// = 'a';
  pixel[check_x+2][0].set(&accelerator_col);// = 'a'; 
  buttons[4].col.set(&accelerator_col);
}

void button_pressed_settings(int id) {
  //brightness
  if (id==1){
    global_brightness_setting++;
    if (global_brightness_setting >= 6){
      global_brightness_setting = 1;
    }
    if (global_brightness_setting == 1)   global_brightness = 0.02;
    if (global_brightness_setting == 2)   global_brightness = 0.1;
    if (global_brightness_setting == 3)   global_brightness = 0.5;
    if (global_brightness_setting == 4)   global_brightness = 0.7;
    if (global_brightness_setting == 5)   global_brightness = 1.0;
//    global_brightness += 0.2;
//    if (global_brightness > 1.1){
//      global_brightness = 0.2;
//    }
  }

  //2 is num players
  if (id == 2){
    num_players++;
    if (num_players > MAX_NUM_PLAYERS){
      num_players = 2;
    }
  }

  //3 is start speed

  //4 is exit
  if (id==4){
    Serial.print("exit settings");
    gameState = STATE_INTRO;
    button_lock_timer = millis() + button_lock_time;
  }
}

void resetMatrix() {
  for (int y = 0; y < NUM_ROWS; y++) {
    for (int x = 0; x < NUM_COLS; x++) {
      pixel[x][y].blank();
    }
  }
}

void setLEDs() {
  boolean anythingChanged = false;

  for (int y = 0; y < NUM_ROWS; y++) {
    for (int x = 0; x < NUM_COLS; x++) {

      //chekc if this pixel has changed
      if (pixel[x][y].has_been_changed){// != last_sent_grid[x][y]) {
        pixel[x][y].has_been_changed = false;
        int this_x = NUM_COLS - 1 - x;
        if (y==1) this_x = NUM_COLS - 1 - x - 1;  //need to shift this one over by 1
        leds[y].setPixelColor(this_x, pixel[x][y].get_uint_dotstar(global_brightness));

//        if (y == 0) pix0.setPixelColor(NUM_COLS - 1 - x , pixel[x][y].get_uint_dotstar(global_brightness));
//        if (y == 1) pix1.setPixelColor(NUM_COLS - 1 - x - 1, pixel[x][y].get_uint_dotstar(global_brightness));
//        if (y == 2) pix2.setPixelColor(NUM_COLS - 1 - x , pixel[x][y].get_uint_dotstar(global_brightness));
//        if (y == 3) pix3.setPixelColor(NUM_COLS - 1 - x , pixel[x][y].get_uint_dotstar(global_brightness));
//        if (y == 4) pix4.setPixelColor(NUM_COLS - 1 - x , pixel[x][y].get_uint_dotstar(global_brightness));

        anythingChanged = true;
      }
    }
  }

  //if any pixels were changed, updated all of the LEDs and store the current grid for comparison next frame
  if (anythingChanged) {
    for (int y = 0; y < NUM_ROWS; y++) {
      leds[y].show();
    }
//    pix0.show();
//    pix1.show();
//    pix2.show();
//    pix3.show();
//    pix4.show();

    //  //store the current grid for comparison
//    for (int y = 0; y < NUM_ROWS; y++) {
//      for (int x = 0; x < NUM_COLS; x++) {
//        last_sent_grid[x][y] = pixel[x][y];
//      }
//    }
  }

  //update the button colors
  bool button_changed = false;
  for (int i=0; i<NUM_BUTTONS; i++){
    if(buttons[i].col.has_been_changed){
      button_pixels.setPixelColor( buttons[i].led_id, buttons[i].col.get_uint());
      buttons[i].col.has_been_changed = false;
      button_changed = true;
    }
  }
  if (button_changed){
    button_pixels.show();
  }
}

/*
void debugDisplay(int x, int y, char col) {
  String line = String(x) + "," + String(y) + "," + pixel[x][y] + "\n";
  debug_display_buffer += line;
  num_queued_debug_display++;

  if (num_queued_debug_display == 8) {
    sendDebugDisplayMessage();
  }

  //Serial.print(line);
}
*/

void sendDebugDisplayMessage() {
  //send it
  Serial.print(debug_display_buffer);

  //reset it
  num_queued_debug_display = 0;
  debug_display_buffer = "";
}
