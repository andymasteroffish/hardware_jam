
void runPlayers() {

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
      playerColor[i] = color(red(playerColor[i]), green(playerColor[i]), blue(playerColor[i]), 100);
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
  int side = int((playerX[player] / cols) * numSides);
  int shiftAmt = shiftH - 1;
  int bottomY = startY[side] + (shiftAmt);
  if (bottomY > rows - 1) bottomY -= rows; //adjust for when it recovers at the top
  println("bottomY " + side + ": " + bottomY);
  
  //hit the top one
  if (playerY[player] == startY[side]) {
    println("hit the down shifter");
    if (playerY[player] + shiftAmt > rows - 1) shiftAmt -= rows;
    playerY[player] += shiftAmt;
  }
  else if (playerY[player] == bottomY) {
     //hit the bottom one
    println("hit the up shifter");
    if (playerY[player] - shiftAmt < 0) shiftAmt -= rows;
    playerY[player] -= shiftAmt;
  }
  playerX[player]+= 1;

/*
  // println("checking " + side);
  if (!action[side]) {
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
*/
}

void oldShiftPlayer(int player) {
  //println("rows/2 = " + rows/2);
  if (playerY[player] == rows / 2) {
    int side = int((playerX[player] / cols) * numSides);
    // println("checking " + side);
    if (!action[side]) {
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
