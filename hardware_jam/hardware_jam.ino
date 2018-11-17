#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_START_PIN 6

#define NUM_COLS 5
#define NUM_ROWS 2

int num_pixels = NUM_COLS * NUM_ROWS;
Adafruit_NeoPixel pixel_rows[NUM_ROWS];

//this is ugly, but putting them in an array from the start keeps failing
Adafruit_NeoPixel pixel_rows0 = Adafruit_NeoPixel(NUM_COLS, NEO_PIXEL_START_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel_rows1 = Adafruit_NeoPixel(NUM_COLS, NEO_PIXEL_START_PIN+1, NEO_GRB + NEO_KHZ800);



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


  Serial.println("I am ready to go.");

}

void loop() {


  //checking for input
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

  //showing the thing
  display_game();

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
