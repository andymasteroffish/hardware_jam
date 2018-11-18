

import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

int cols = 90;
int rows = 2;
char[][] grid = new char[cols][rows];
char[][] last_sent_grid = new char[cols][rows];


int box_size = 10;

void setup() 
{
  size(900, 20);
  
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
    println("got:"+myPort.read());         
  }
  
  //draw the grid
  stroke(0);
  for (int y=0; y<rows; y++){
    for (int x=0; x<cols; x++){
      if (grid[x][y] == '-')  fill(100);
      if (grid[x][y] == 'r')  fill(200,0,0);
      if (grid[x][y] == 'g')  fill(0,200,0);
      if (grid[x][y] == 'b')  fill(0,0,200);
      
      rect(x*box_size, y*box_size, box_size, box_size);
    }
  }
  
  send_pixels();
  
}



void send_pixels(){
  //get the difference betwene current grid and last sent grid
  for (int y=0; y<rows; y++){
    for (int x=0; x<cols; x++){
      if (grid[x][y] != last_sent_grid[x][y]){
        String line =x + "," + y + "," + grid[x][y]+"\n";
        println("send:"+line);
        myPort.write(line);
      }
    }
  }
  
  //store the current grid for comparison
  for (int y=0; y<rows; y++){
    for (int x=0; x<cols; x++){
      last_sent_grid[x][y] = grid[x][y];
    }
  }
}

void keyPressed(){
  
}

//using the mouse to modify the testing grid
void mousePressed(){
  int x = mouseX/box_size; 
  int y = mouseY/box_size; 
  
  if (x < cols && y < rows){
    println(grid[x][y]);
     if (grid[x][y] == '-')         grid[x][y] = 'r'; 
     else if (grid[x][y] == 'r')    grid[x][y] = 'g';
     else if (grid[x][y] == 'g')    grid[x][y] = 'b';
     else if (grid[x][y] == 'b')    grid[x][y] = '-';
  }
  
  println(x+" "+y);
}