void updateSides() {


  /* 
   //assign randomly
   //create blocks 
   int numBlocks = 3;
   for (int i = 0; i < numBlocks; i++) {
   int randoCol = int(random(cols));
   while (pixel[0][randoCol] == 'b') { //reassign to avoid duplicates
   println("reassigning block due to duplicate");
   randoCol = int(random(cols));
   }
   for (int j =0; j < blockH; j++) {
   //assigning to fill out the blocks rows
   pixel[j][randoCol] = 'b';
   }
   }
   */

  for (int i = 0; i < numSides; i++) {
    switch (sideAction[i]) {

    case 0: //blocks
      updateBlock(i);
      break;

    case 1:  
      updateShifter(i);

      break;

    case 2:
      updateSpeeder(i);

      break;

    case 3:
      updateDir(i);
      break;
    }
  }
}

void updateBlock(int side) {

  int middle = sideW / 2;
  int col = side * sideW + middle;


  resetCol(col);
  //draw out the col
  for (int j = 0; j < blockH; j++) {
    //assigning to fill out the blocks rows
    if (action[side]) {
      pixel[j][col] = 'b'; //up
    } else {
      pixel[j + rows/2][col] = 'b'; //down
    }
  }
}

void updateShifter(int side) {

  //int shifterH = 3;
  int middle = sideW / 2;
  int col = side * sideW + middle;

  resetCol(col);
  
  pixel[rows / 2][col] = 'c'; //middle
  if (action[side]) { //
    pixel[0][col] = 'c'; //top
  } else {
    pixel[rows - 1][col] = 'c'; //bottom
  }
 
}
void updateSpeeder(int side) {
  
}
void updateDir(int side) {
  
}
