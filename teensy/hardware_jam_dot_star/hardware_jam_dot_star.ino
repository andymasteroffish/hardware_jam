#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

struct Button{
  int pin;
  int next_check_time;  //for debounce
  bool is_held;
  char key;
};



#define NUM_COLS 85
#define NUM_ROWS 3
 
//this is ugly, but putting them in an array from the start keeps failing
//data pin then clock pin
//data is green, clock is yellow
Adafruit_DotStar pix0 = Adafruit_DotStar(NUM_COLS, 4,5 , DOTSTAR_BRG);
Adafruit_DotStar pix1 = Adafruit_DotStar(NUM_COLS, 6,7, DOTSTAR_BRG);
Adafruit_DotStar pix2 = Adafruit_DotStar(NUM_COLS, 11, 12, DOTSTAR_BRG);
Adafruit_DotStar pix3 = Adafruit_DotStar(NUM_COLS, 13, 14, DOTSTAR_BRG);
Adafruit_DotStar pix4 = Adafruit_DotStar(NUM_COLS, 15, 16, DOTSTAR_BRG);

//getting serial
char buff[256];
int buff_pos = 0;

//buttons
int debounce_time = 30; //millis
#define NUM_BUTTONS 8
Button buttons[NUM_BUTTONS];


//test states
int player_x = 0;
int player_y = 0;




void setup() {
  Serial.begin(9600); 

  //actually throw the pixel rows into an array
  //pix[0] = pix0;
  //pix[1] = pix1;
  
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

  
  for (int i=0; i<NUM_BUTTONS; i++){
    buttons[i].next_check_time = 0;
    buttons[i].is_held = false;
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  Serial.println("I am ready to go.");

}



void loop() {

  //checking serial
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
  pix0.show();
  pix1.show(); 
  pix2.show(); 
  pix3.show(); 
  pix4.show(); 

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
          Keyboard.print(buttons[i].key);
        }
        
      }
      
    }
  }
}


//sets an individual pixel value but does not update the strand. call show() to do that
void set_pixel(int x, int y, char col_char){
    uint32_t color = 0x000000;  //'a'

    if (col_char == 'b') color = 0x004400;  //blocked
    if (col_char == 'c') color = 0x444444;  //shifter
    if (col_char == 'd') color = 0x440000;  //accelerator
    if (col_char == 'e') color = 0x444400;  //reverse

    if (col_char == '1') color = 0x000088;  //p1
    if (col_char == '2') color = 0x008888;  //p2
    if (col_char == '3') color = 0x000044;  //p3
  
    if (y==0) pix0.setPixelColor(x, color);
    if (y==1) pix1.setPixelColor(x, color);
    if (y==2) pix2.setPixelColor(x, color);
    if (y==3) pix3.setPixelColor(x, color);
    if (y==4) pix4.setPixelColor(x, color);
}
