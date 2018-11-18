
import java.util.Arrays;

int cols = 88;
int rows = 3;
int numPlayers = 3; //up to 5 players? mebbes?
int numSides = 8;
float startSpeed = 0.25;

int blockH = int((2.0/3.0) * rows);
int shiftH = blockH;
int sideW = cols / numSides; //pixels per side
boolean[] action = new boolean[numSides];
int[] sideAction = new int[numSides]; //actions: 0 = blockToggle; 1 = shifter (true = up position, false = down position?); 2 = accelerator/decelerator? 3 = reversal?

char[][] pixel = new char[rows][cols];
int pixelScale = 1;
float[] playerX = new float[numPlayers];
int[] playerY = new int[numPlayers];
float[] playerSpeed = new float[numPlayers];

color[] playerColor = new color[numPlayers];




void setup() {
  size(3, 5);//800, 500);
  println("blockh = " + blockH);
  pixelScale = width / cols;

  //default states
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      pixel[i][j] = 'a';
    }
  }

  for (int i = 0; i < numPlayers; i++) {
    playerX[i] = i; //separated so they dont overlap
    //playerY[i] = i * (rows - 1); //put on opposite side
    playerY[i] = i; //use its own lane
    playerSpeed[i] = startSpeed;

    playerColor[i] = color(random(255), random(255), random(255));
  }

  for (int i = 0; i < numSides; i++) {
    action[i] = false;
  }
  //hard assigning the input actions
  sideAction[0] = 1;
  sideAction[1] = 1;
  sideAction[2] = 1;
  sideAction[3] = 1;
  sideAction[4] = 1;
  sideAction[5] = 0;
  sideAction[6] = 1;
  sideAction[7] = 1;

  updateSides(); //update interaction side elements
}

void draw() {
  //processing stuff
  background(0);
  stroke(255);
  ellipseMode(CORNER);

  displayBoard(); //board before any players
  checkInput(); //using keypressed in processing ... only run this if there is new input?

  runPlayers();
  output();
}



void displayBoard() {

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      switch (pixel[y][x]) {
      case 'a': //empty
        fill((x / sideW) * (255 / numSides));
        break;
      case 'b': //blocked
        fill(255, 0, 0);
        break;
      case 'c': //shifter
        fill(0, 0, 255);
        break;
      }


      //draw out the pixel
      ellipse(x * pixelScale, y * pixelScale, pixelScale, pixelScale);
    }
  }
}

//void switchAction() {
//}

void resetCol(int col) {
  for (int i = 0; i < rows; i++) {
    pixel[i][col] = 'a';
  }
}

void checkInput() {
  //check input
}
void keyPressed() {

  switch(key) { //hardcoded for now
  case '0':
    action[0] = !action[0];
    break;
  case '1':
    action[1] = !action[1];
    break;
  case '2':
    action[2] = !action[2];
    break;
  case '3':
    action[3] = !action[3];
    break;
  case '4':
    action[4] = !action[4];
    break;
  case '5':
    action[5] = !action[5];
    break;
  case '6':
    action[6] = !action[6];
    break;
  case '7':
    action[7] = !action[7];
    break;
  }
  updateSides(); //update block positions
  //println(action);
}

void output(){
   
  //char[][] pixelPlay = pixel.clone();
  
  char [][] pixelPlay = new char[pixel.length][];
  for(int i = 0; i < pixel.length; i++)
      pixelPlay[i] = pixel[i].clone();
  //pixelPlay[0][3] = 'z';
  
  //idk why this is affecting the original array
  for (int i = 0; i < numPlayers; i++) {
    pixelPlay[playerY[i]][(int)playerX[i]] = Integer.toString(i).charAt(0);
   // println(pixelPlay[playerY[i]][(int)playerX[i]] + " old: " + pixel[playerY[i]][(int)playerX[i]]);
  }
  String output = "";
  for (int y = 0; y < rows; y++){
   for (int x = 0; x < cols; x++){
     output += pixelPlay[y][x];
   }
  }
  println(output);
}
