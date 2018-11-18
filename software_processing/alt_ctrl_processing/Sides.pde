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
      updateAcc(i);

      break;

    case 3:
      updateDir(i);
      break;
    }
  }
}
void updateSide(int side) {

  switch (sideAction[side]) {
  case 0: //blocks
    updateBlock(side);
    break;
  case 1:  
    updateShifter(side);
    break;
  case 2:
    updateAcc(side);
    break;
  case 3:
    updateDir(side);
    break;
  }
}


void updateBlock(int side) {

  int middleX = sideW / 2;
  int col = side * sideW + middleX;


  resetCol(col);
  //draw out the col
  for (int j = 0; j < blockH; j++) {
    int whichRow = j + startY[side];
    if (whichRow > rows - 1) whichRow -= rows;
    pixel[whichRow][col] = 'b';
  }
}

void updateShifter(int side) {

  //int shifterH = 3;
  int middle = sideW / 2;
  int col = side * sideW + middle;

  resetCol(col);

  //use this method to color the only the outside


  int whichRow = startY[side]; //top
  pixel[whichRow][col] = 'c';
  whichRow += shiftH - 1; //bottom
  if (whichRow > rows - 1) whichRow -= rows; //correct for overshoot
  pixel[whichRow][col] = 'c';



  /*
//use this method to color the interior
   for (int i = 0; i < shiftH; i++) {
   
   int whichRow =i + startY[side];
   if (whichRow > rows - 1) whichRow -= rows;
   pixel[whichRow][col] = 'c';
   }
   */
}
void updateAcc(int side) {
  
  int middleX = sideW / 2;
  int col = side * sideW + middleX;


  resetCol(col);
  //draw out the col
  for (int j = 0; j < accH; j++) {
    int whichRow = j + startY[side];
    if (whichRow > rows - 1) whichRow -= rows;
    pixel[whichRow][col] = 'd';
  }
}
void updateDir(int side) {
  
  int middleX = sideW / 2;
  int col = side * sideW + middleX;
  
  resetCol(col);
  pixel[startY[side]][col] = 'e';

}
