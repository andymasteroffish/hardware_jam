
import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
boolean useSerial = false;

int cols = 85; //should be divisible by 8
int rows = 3;
int numPlayers = 2; //up to 5 players? mebbes?
int numSides = 8;
float startSpeed = 0.1;//0.02;
float speedMult = 1.1;
int winner = -1;
boolean gameOver = false;
boolean gameBegun = false;

int blockH = int((1.0/3.0) * rows);
int shiftH = 2;
int accH = 1;
int sideW = cols / numSides; //pixels per side
boolean[] action = new boolean[numSides];
boolean[] pAction = new boolean[numSides];
int[] startY = new int[numSides];
int[] sideAction = new int[numSides]; //actions: 0 = blockToggle; 1 = shifter (true = up position, false = down position?); 2 = accelerator/decelerator? 3 = reversal?

char[][] pixel = new char[rows][cols];
char[][] last_sent_grid = new char[cols][rows]; //reversed because i did y/x made more sense to me for some odd reason

int pixelScale = 1;
float[] playerX = new float[numPlayers];
int[] playerY = new int[numPlayers];
float[] playerSpeed = new float[numPlayers];

color[] playerColor = new color[numPlayers];

int[] visibleX = new int[numPlayers];
int visibleRad = 10;



void setup() {
  size(3400, 300);
  if (useSerial && !gameOver) {
    for (int i=0; i<Serial.list().length; i++) {
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
  }

  gameOver = false;
  gameBegun = false;
  winner = -1;

  pixelScale = width / cols;

  //default states
  resetMatrix(); 

  for (int i = 0; i < numPlayers; i++) {
    playerX[i] = i; //separated so they dont overlap
    if (numPlayers == 2) playerY[i] = i * (rows - 1); //put on opposite side 
    else playerY[i] = i; //use its own lane
    playerSpeed[i] = startSpeed;

    playerColor[i] = color(random(255), random(255), random(255));
  }

  for (int i = 0; i < numSides; i++) {
    action[i] = false;
    pAction[i] = action[i];
    startY[i] = 0;
  }
  //hard assigning the input actions
  sideAction[0] = 1;
  sideAction[1] = 1;
  sideAction[2] = 2;
  sideAction[3] = 1;
  sideAction[4] = 2;
  sideAction[5] = 3;
  sideAction[6] = 1;
  sideAction[7] = 0;

  updateSides(); //update interaction side elements
}

void draw() {
  //processing stuff
  background(0);
  stroke(80);
  ellipseMode(CORNER);

  displayBoard(); //board before any players

  //checkInput(); //using keypressed in processing ... only run this if there is new input?
  if (!gameBegun) displayIntro();
  else if (!gameOver) runPlayers();
  else displayWinner(winner);
  output();
}



void displayBoard() {


  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      switch (pixel[y][x]) {
      case 'a': //empty
        fill(0);
        //fill((x / sideW) * (255 / numSides)); //differently colored sides
        break;
      case 'b': //blocked
        fill(255, 0, 0);
        break;
      case 'c': //shifter
        fill(0, 0, 255);
        break;
      case 'd': //accelerator
        fill(0, 255, 0);
        break;
      case 'e': //reverseDir
        fill(255, 0, 255);
        break;
      case '0': //PLAYER 0 WON
        fill(playerColor[0]);
        break;
      case '1': //PLAYER 1 WON
        fill(playerColor[1]);
        break;
      case '2': //PLAYER 2 WON
        if (numPlayers >= 3) fill(playerColor[2]);
        break;
      }

      //processing
      if (isVisible(0, x) || isVisible(1, x)) {
       stroke(255);
      } else {
        fill(0);
        stroke(30);
      } 
      
      //draw out the pixel
      ellipse(x * pixelScale, y * pixelScale, pixelScale/1.7, pixelScale/1.7);
    }
  }
}


void resetCol(int col) {
  for (int i = 0; i < rows; i++) {
    pixel[i][col] = 'a';
  }
}


void displayIntro() {

  int mod = (frameCount/3) % cols;
  String abc = "abcde123";
  println("mod=" + mod);

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      int loc = x + mod + y * cols;
      pixel[y][x] = abc.charAt(loc % abc.length());
      //print(pixel[y][x]);
      //if (loc % cols == mod) pixel[y][x] = 'a';
      //pixel[i][j] = Integer.toString(player).charAt(0);
    }
  }
}

void resetMatrix() {

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      pixel[i][j] = 'a';
    }
  }
  updateSides();
}
