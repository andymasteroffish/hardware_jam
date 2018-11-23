/*

This applicaiton is used to debug the version of the game that runs entirely on the teensy
It tries to simulate the LED strips

*/

import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

int cols = 85;
int rows = 5;
char[][] grid = new char[cols][rows];

//getting serial
char[] buff = new char[256];
int buff_pos = 0;

int box_size = 10;


void setup() 
{
  size(850, 50);
  
  for (int i=0; i<Serial.list().length; i++){
    println(i+":"+Serial.list()[i]);
  }
  
  println("try to connect");
  
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  
  println("done serial setup");
  
  for (int x=0; x<cols; x++){
    for (int y=0; y<rows; y++){
      grid[x][y] = '-';
    }
  }
}

void draw() {
  background(255);
  
  //read from serial
  if ( myPort.available() > 0) {  // If data is available,
    buff[buff_pos] = (char)myPort.read();  
    //incoming messages are in this format "x,y,char\n"
    //you need the new line char for it to count
    if (buff[buff_pos] == '\n') {
      println("got one");
      int led_x = -1;
      int led_y = -1;
      char led_col = 'X';
      
      int cur_info = 0;
      String this_string = "";
      for (int i=0; i<=buff_pos; i++){
        if (buff[i] == ','){
          
          if (cur_info == 0){
            led_x = Integer.parseInt(this_string);
          }
          else if (cur_info == 1){
            led_y = Integer.parseInt(this_string); 
          }
          
         this_string = "";
          cur_info++;
        }
        else{
          this_string += buff[i]; 
        }
      }
      
      //the color is the last thing. No comma after it
      led_col = this_string.charAt(0);
      
      println(led_x+","+led_y+":"+led_col);
      if (led_x >= 0 && led_x < cols && led_y >= 0 && led_y < rows){
        grid[led_x][led_y] = led_col;
      }
      
      buff_pos = 0;
    } else {
      buff_pos++;
    }
  }
  
  
  
  
  //draw the grid
  stroke(0);
  for (int y=0; y<rows; y++){
    for (int x=0; x<cols; x++){
      switch (grid[x][y]) {
      case '-': //empty
        fill(0, 0, 0);
        break;
      case 'b': //blocked
        fill(255, 0, 0);
        break;
      case 's': //shifter
        fill(255, 255, 255);
        break;
      case 'a': //accelerator
        fill(0, 255, 0);
        break;
      case 'r': //reverseDir
        fill(255, 0, 255);
        break;
      case '0': 
        fill(255, 255, 0);
        break;
      case '1': 
        fill( 0, 200, 200);
        break;
      }
      
      rect(x*box_size, y*box_size, box_size, box_size);
    }
  }
  
}