#include <Adafruit_NeoPixel.h>

struct Button{
  int pin;
  int next_check_time;  //for debounce
  bool is_on;
};

#define NEO_PIXEL_START_PIN 6

#define NUM_COLS 5
#define NUM_ROWS 2

int num_pixels = NUM_COLS * NUM_ROWS;
Adafruit_NeoPixel pixel_rows[NUM_ROWS];

//this is ugly, but putting them in an array from the start keeps failing
Adafruit_NeoPixel pixel_rows0 = Adafruit_NeoPixel(NUM_COLS, NEO_PIXEL_START_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel_rows1 = Adafruit_NeoPixel(NUM_COLS, NEO_PIXEL_START_PIN+1, NEO_GRB + NEO_KHZ800);

//buttons
int debounce_time = 50; //millis
Button test_button;


//test states
int player_x = 0;
int player_y = 0;




void setup() {
  Serial.begin(9600); 

  //actually throw the pixel rows into an array
  pixel_rows[0] = pixel_rows0;
  pixel_rows[1] = pixel_rows1;
  
  for (int i=0; i<NUM_ROWS; i++){
    pixel_rows[i].begin();
  }

  //buttons
  test_button.pin = 2;
  test_button.next_check_time = 0;
  test_button.is_on = false;
  pinMode(test_button.pin, INPUT_PULLUP);

  Serial.println("I am ready to go.");

}

void loop() {


  //checking for serial test input
  if (Serial.available() > 0) {
    String incoming = Serial.readString();
    Serial.println(incoming);
    if (incoming[0] == 'w'){
      player_y++;
      if (player_y >= NUM_ROWS) player_y = 0;
    }
    if (incoming[0] == 's'){
      player_y--;
      if (player_y < 0) player_y = NUM_ROWS-1;
    }
    if (incoming[0] == 'd'){
      player_x++;
      if (player_x >= NUM_COLS) player_x = 0;
    }
    if (incoming[0] == 'a'){
      player_x--;
      if (player_x < 0) player_x = NUM_COLS-1;
    }

    Serial.println("player at "+String(player_x)+" , "+String(player_y));
  
  }

  //checking button input
  check_input();

  //showing the thing
  display_game();

}

void check_input(){

  //this should be an array of buttons
  if (millis() > test_button.next_check_time){
    //get the current value
    bool cur_val = false;
    if (digitalRead(test_button.pin) == LOW){
      cur_val = true;
    }

    //is that different form the current value?
    if (cur_val != test_button.is_on){
      test_button.is_on = cur_val;
      test_button.next_check_time = millis() + debounce_time;
      Serial.println("button: "+String(test_button.is_on));
      if (test_button.is_on){
        player_x++;
        if (player_x >= NUM_COLS) player_x = 0;
      }
    }
    
  }
}

void display_game(){
  for (int x=0; x<NUM_COLS; x++){
    for (int y=0; y<NUM_ROWS; y++){

      int col_r = 20;
      int col_g = 0;
      int col_b = 0;

      if (player_x == x && player_y == y){
        col_g = 55;
      }

       pixel_rows[y].setPixelColor(x, col_r,col_g,col_b);
    
    }
  }

  for (int y=0; y<NUM_ROWS; y++){
    pixel_rows[y].show();
  }
}
