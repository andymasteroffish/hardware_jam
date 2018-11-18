
void checkInput() {
  //check input
  if (keyPressed) {
    
    for (int i = 0; i < numSides; i++){
      if (key == Integer.toString(i).charAt(0)){ //convert each side into a char
        
        action[i] = true;
        if (action[i] != pAction[i]) { //newly pressed
          startY[i]++;
          if (startY[i] > rows - 1) startY[i] = 0;
          updateSide(i);
          
        }
      } else {
       action[i] = false; 
      }
      
    }
  } else {
    for (int i = 0; i < numSides; i++)      action[i] = false; 
  }
    

  updateActionRecord();
}
void keyPressed() {
  /*
//the desktop version which can be called only upon keypress
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
   //updateSides(); //update block positions
   //println(action);
   */
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

  //idk why this is affecting the original array
  for (int i = 0; i < numPlayers; i++) {
    pixelPlay[playerY[i]][(int)playerX[i]] = Integer.toString(i).charAt(0);
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
}
