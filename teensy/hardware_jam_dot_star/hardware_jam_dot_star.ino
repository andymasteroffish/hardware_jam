#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

struct Button{
  int pin;
  int next_check_time;  //for debounce
  bool is_held;
};

#define DOT_STAR_START_PIN 4

#define NUM_COLS 90
#define NUM_ROWS 2

//this is ugly, but putting them in an array from the start keeps failing
Adafruit_DotStar pixel_rows0 = Adafruit_DotStar(NUM_COLS, DOT_STAR_START_PIN+0, DOT_STAR_START_PIN+1, DOTSTAR_BRG);
Adafruit_DotStar pixel_rows1 = Adafruit_DotStar(NUM_COLS, DOT_STAR_START_PIN+2, DOT_STAR_START_PIN+3, DOTSTAR_BRG);

//getting serial
char buff[256];
int buff_pos = 0;

//buttons
int debounce_time = 30; //millis
#define NUM_BUTTONS 2
Button buttons[NUM_BUTTONS];


//test states
int player_x = 0;
int player_y = 0;




void setup() {
  Serial.begin(9600); 

  //actually throw the pixel rows into an array
  //pixel_rows[0] = pixel_rows0;
  //pixel_rows[1] = pixel_rows1;
  
   pixel_rows0.begin();
   pixel_rows0.show();

   pixel_rows1.begin();
   pixel_rows1.show();
  

  //buttons
  buttons[0].pin = 2;
  buttons[1].pin = 20;
  
  for (int i=0; i<NUM_BUTTONS; i++){
    buttons[i].next_check_time = 0;
    buttons[i].is_held = false;
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  Serial.println("I am ready to go.");

}



void loop() {

  //chekcing serial
  while (Serial.available() > 0) {
    buff[buff_pos] = Serial.read();

    //incoming messages are in this format "x,y,char\n"
    //you need the new line char for it to count
    if (buff[buff_pos] == '\n') {
      int led_x, led_y;
      char led_col;
      sscanf(buff, "%d,%d,%c", &led_x, &led_y, &led_col);
      
      set_pixel(led_x, led_y, led_col);

      //Serial.write("hi");
      
      buff_pos = 0;
    } else {
      buff_pos++;
    }
  }


  //checking for serial test input
//  if (Serial.available() > 0) {
//    String incoming = Serial.readString();
//    Serial.println(incoming);
//    if (incoming[0] == 'w'){
//      player_y++;
//      if (player_y >= NUM_ROWS) player_y = 0;
//    }
//    if (incoming[0] == 's'){
//      player_y--;
//      if (player_y < 0) player_y = NUM_ROWS-1;
//    }
//    if (incoming[0] == 'd'){
//      player_x++;
//      if (player_x >= NUM_COLS) player_x = 0;
//    }
//    if (incoming[0] == 'a'){
//      player_x--;
//      if (player_x < 0) player_x = NUM_COLS-1;
//    }
//
//    Serial.println("player at "+String(player_x)+" , "+String(player_y));
//  
//  }

  //checking button input
  check_input();

  //showing the thing
  //display_game();
  pixel_rows0.show();
  pixel_rows1.show(); 

}

void check_input(){

  for (int i=0; i<NUM_BUTTONS; i++){
    if (millis() > buttons[i].next_check_time){
      //get the current value
      bool cur_val = false;
      if (digitalRead(buttons[i].pin) == LOW){
        cur_val = true;
      }
  
      //is that different form the current value?
      if (cur_val != buttons[i].is_held){
        buttons[i].is_held = cur_val;
        buttons[i].next_check_time = millis() + debounce_time;
        Serial.println("button: "+String(buttons[i].is_held));
        if (buttons[i].is_held){
          if (i==0){
            player_x++;
            if (player_x >= NUM_COLS) player_x = 0;
          }
          if (i==1){
            player_y++;
            if (player_y >= NUM_ROWS) player_y = 0;
          }
        }
        
      }
      
    }
  }
}

void display_from_string(String line){
  for (int x=0; x<NUM_COLS; x++){
    for (int y=0; y<NUM_ROWS; y++){

      //int char_pos = ;

      char this_char = line[x + y * NUM_COLS];

      uint32_t color = 0x111111;

      if (this_char == 'r') color = 0x440000;
      if (this_char == 'g') color = 0x004400;
      if (this_char == 'b') color = 0x000044;

      if (y==0) pixel_rows0.setPixelColor(x, color);
      if (y==1) pixel_rows1.setPixelColor(x, color);

       //pixel_rows[y].setPixelColor(x, color);
    
    }
  }

  pixel_rows0.show();
  pixel_rows1.show(); 
}

//sets an individual pixel value but does not update the strand. call show() to do that
void set_pixel(int x, int y, char col_char){
    uint32_t color = 0x111111;

    if (col_char == 'r') color = 0x440000;
    if (col_char == 'g') color = 0x004400;
    if (col_char == 'b') color = 0x000044;
  
    if (y==0) pixel_rows0.setPixelColor(x, color);
    if (y==1) pixel_rows1.setPixelColor(x, color);
}


//JUST FOR TESTING
void display_game(){
  for (int x=0; x<NUM_COLS; x++){
    for (int y=0; y<NUM_ROWS; y++){

      uint32_t color = 0x110000;

      if (player_x == x && player_y == y){
        color = 0x004400;
      }

      if (y==0) pixel_rows0.setPixelColor(x, color);
      if (y==1) pixel_rows1.setPixelColor(x, color);

       //pixel_rows[y].setPixelColor(x, color);
    
    }
  }

  pixel_rows0.show();
  pixel_rows1.show();
}
