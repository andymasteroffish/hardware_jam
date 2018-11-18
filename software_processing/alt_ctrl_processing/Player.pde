
void runPlayers(){

  //override those values with player
  for (int i = 0; i < numPlayers; i++) {
    fill(playerColor[i]);

    char current = pixel[playerY[i]][(int)playerX[i]];
    switch (current) {
    case 'a': //unobstructed
      //move normally
      moveFwd(i);
      break;

    case 'b': //blocked
      playerSpeed[i] = 0; //placeholder for end player movement/game?
      break;

    case 'c': //shift
      shiftPlayer(i);

      break;
    }

    ellipse(playerX[i] * pixelScale, playerY[i] * pixelScale, pixelScale, pixelScale);
  } 
}
void moveFwd(int player) {
  if (playerX[player] < cols - 1)
    playerX[player] += playerSpeed[player];
  else playerX[player] = 0;
}

void shiftPlayer(int player) {
  //println("rows/2 = " + rows/2);
  if (playerY[player] == rows / 2){
    int side = int((playerX[player] / cols) * numSides);
   // println("checking " + side);
     if (!action[side]){
       //println("move player " + player + " up");
       playerY[player] += rows / 2; //false = move from middle to top
     } else {
       //println("move player " + player + " down");
       playerY[player] -= rows / 2; //true = move from middle to bottom
     }
  } else {
    
    if (playerY[player] == 0) {
      
       //println("move player " + player + " down");
      playerY[player] += rows / 2; //move down from top
    }
    if (playerY[player] == rows - 1) {
      
      // println("move player " + player + " up");
      playerY[player] -= rows / 2; //move up from bottom
    }
  }
  //moveFwd(player);
  playerX[player] += 1; //send a full position so it doesnt recheck it if speed is < 1
}
