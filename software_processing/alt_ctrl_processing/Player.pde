
void runPlayers() {
  
   winner = checkWinners(); //winner remains -1 if no winner
   if (winner != -1) displayWinner(winner);

  //override those values with player
  for (int i = 0; i < numPlayers; i++) {
    playerSpeed[i] = constrain(playerSpeed[i], -0.99f, 0.99f); //prevent it from skipping things
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
    case 'd': //acc
      accPlayer(i);
      break;
    case 'e': //reverse dir
      reverseDir(i);
      break;
    }


    ellipse(playerX[i] * pixelScale, playerY[i] * pixelScale, pixelScale, pixelScale);
  }
}
void moveFwd(int player) {
  if (playerX[player] < cols - 1) {
    if (playerX[player] >= 0) {
      playerX[player] += playerSpeed[player];
    } else {
      playerX[player] = cols - 2; //went off the left
    }
  } else { //went off the right
    playerX[player] = 0;
  }
}
void shiftPlayer(int player) {
  int side = int((playerX[player] / cols) * numSides);
  int shiftAmt = shiftH - 1;
  int bottomY = startY[side] + (shiftAmt);
  if (bottomY > rows - 1) bottomY -= rows; //adjust for when it recovers at the top
  //println("bottomY " + side + ": " + bottomY);

  //hit the top one
  if (playerY[player] == startY[side]) {
    //println("hit the down shifter");
    if (playerY[player] + shiftAmt > rows - 1) shiftAmt -= rows;
    playerY[player] += shiftAmt;
  } else if (playerY[player] == bottomY) {
    //hit the bottom one
    //println("hit the up shifter");
    if (playerY[player] - shiftAmt < 0) shiftAmt -= rows;
    playerY[player] -= shiftAmt;
  }
  if (playerSpeed[player] > 0) playerX[player]+= 1;
  else playerX[player] -= 1;
}

void accPlayer(int player) {
  playerSpeed[player] *= speedMult;
  moveFwd(player);
}

void reverseDir(int player) {
  playerSpeed[player] *= -1;
  playerX[player] += playerSpeed[player]; //to avoid getting stuck in a loop
}

int checkWinners() { //returns winner index if won
  int winPlayer = -1;
  int playersAlive = 0;
  for (int i = 0; i < numPlayers; i++) {
    if (playerSpeed[i] != 0) {
      playersAlive++;
      winPlayer = i;
    }
  }
  if (playersAlive == 1) {
    println("winner found (player " + winPlayer);
    return winPlayer;
  }
  else return -1;
}

void displayWinner(int player) {
  println("PLAYER " + player + " WON!!!");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      pixel[i][j] = Integer.toString(player).charAt(0);
    }
  }
  //fill(playerColor[player]);
  gameOver = true;
}
