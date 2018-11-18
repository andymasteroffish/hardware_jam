

import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

int cols = 90;
int rows = 2;
char[][] grid = new char[cols][rows];



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
    println(myPort.read());         
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
  String line = "";
  for (int y=0; y<rows; y++){
    for (int x=0; x<cols; x++){
      line += grid[x][y];
    }
  }
  line += '\n';
  
  myPort.write(line);
}

void keyPressed(){
 // println("sending");
  //myPort.write("---r-g----");
  //myPort.write('\n');
  
  
}

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


/*
  // Wiring/Arduino code:
 // Read data from the serial and turn ON or OFF a light depending on the value
 
 char val; // Data received from the serial port
 int ledPin = 4; // Set the pin to digital I/O 4
 
 void setup() {
 pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
 Serial.begin(9600); // Start serial communication at 9600 bps
 }
 
 void loop() {
 while (Serial.available()) { // If data is available to read,
 val = Serial.read(); // read it and store it in val
 }
 if (val == 'H') { // If H was received
 digitalWrite(ledPin, HIGH); // turn the LED on
 } else {
 digitalWrite(ledPin, LOW); // Otherwise turn it OFF
 }
 delay(100); // Wait 100 milliseconds for next reading
 }
 
 */