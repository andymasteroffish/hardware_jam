void visibleInput() {
  if (keyPressed) {
    switch (key) {
    case 'a': 
      visibleX[0]--;
      break;
    case 'd': 
      visibleX[0]++;
      break;
    case 'j': 
      visibleX[1]--;
      break;
    case 'l': 
      visibleX[1]++;
      break;
    }
    for (int i = 0; i < numPlayers; i++) {
      if (visibleX[i] < 0) visibleX[i] = cols - 1;
      if (visibleX[i] > cols- 1) visibleX[i] = 1;
    }
  }
}

//just for processing
boolean isVisible(int player, int pos) {
  if (abs(visibleX[player] - pos) > visibleRad) return false; 
  else return true;
}

void checkInput() {

  visibleInput(); //processing

  for (int i = 0; i < numSides; i++) {
    if (key == (Integer.toString(i).charAt(0))) { //convert each side into a char

      action[i] = true;
      if (action[i] != pAction[i]) { //newly pressed
        //println("new key= " + key);
        startY[i]++;
        if (startY[i] > rows - 1) startY[i] = 0;
        updateSide(i);
      }
    } else {
      action[i] = false;
    }
  }

  updateActionRecord();
}

void keyReleased() {

  //println("key " + key + " released");

  for (int i = 0; i < numSides; i++) {
    if (key == (Integer.toString(i).charAt(0))) { //convert each side into a char

      action[i] = false;
      if (action[i] != pAction[i]) { //newly pressed
        //startY[i]++;
        //if (startY[i] > rows - 1) startY[i] = 0;
        // updateSide(i);
      }
    }
  }

  updateActionRecord();
}
void keyPressed() {
  if (!gameBegun) {
    gameBegun = true;
    resetMatrix();
  }
  if (gameOver) setup(); 

  checkInput();
}

void updateActionRecord() {
  for (int i = 0; i < numSides; i++)
    pAction[i] = action[i];
}

void output() {

  //char[][] pixelPlay = pixel.clone();

  char [][] pixelPlay = new char[pixel.length][];
  for (int i = 0; i < pixel.length; i++)
    pixelPlay[i] = pixel[i].clone();
  //pixelPlay[0][3] = 'z';

  for (int i = 0; i < numPlayers; i++) {
    pixelPlay[playerY[i]][(int)playerX[i]] = Integer.toString(i + 1).charAt(0);
    // println(pixelPlay[playerY[i]][(int)playerX[i]] + " old: " + pixel[playerY[i]][(int)playerX[i]]);
  }
  String output = "";
  String rowContents[] = new String[rows];
  for (int y = 0; y < rows; y++) {
    rowContents[y] = "";
    for (int x = 0; x < cols; x++) {

      rowContents[y] += pixelPlay[y][x];
    }
    //println(rowContents[y]);
    output += rowContents;
  }
  //println();
  //println(output);



  //get the difference betwene current grid and last sent grid
  for (int y=0; y<rows; y++) {
    for (int x=0; x<cols; x++) {
      if (pixelPlay[y][x] != last_sent_grid[x][y]) {
        String line =x + "," + y + "," + pixelPlay[y][x]+"\n";
        //println("send:"+line);

        if (useSerial) {
          myPort.write(line);
        }
      }
    }
  }

  //store the current grid for comparison
  for (int y=0; y<rows; y++) {
    for (int x=0; x<cols; x++) {
      last_sent_grid[x][y] = pixelPlay[y][x];
    }
  }
}
