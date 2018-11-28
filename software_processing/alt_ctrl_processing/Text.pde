

char[][] convertChar(char thisChar, char yes) { //yesis what char to return
  char[][] charMatrix = new char[5][5];
  switch (thisChar){
    case 'a':
      char[][] a = {     {' ',' ',yes,' ',' '},
                         {' ',yes,' ',yes,' '},
                         {' ',yes,yes,yes,' '},
                         {yes,' ',' ',' ',yes},
                         {yes,' ',' ',' ',yes}  };
                         
      charMatrix = clone2D(a);
      break;
    case 'b':
      char[][] b = {     {yes,yes,yes,yes,' '},
                         {yes,' ',' ',' ',yes},
                         {yes,yes,yes,yes,' '},
                         {yes,' ',' ',' ',yes},
                         {yes,yes,yes,yes,' '}  };
                         
      charMatrix = clone2D(b);
      break;
     case 'c':
       char[][] c = {     {' ',yes,yes,yes,' '},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',yes},
                          {' ',yes,yes,yes,' '}  };
       charMatrix = clone2D(c);
      break;
     case 'd':
       char[][] d = {     {yes,yes,yes,yes,' '},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,yes,yes,yes,' '}  };
       charMatrix = clone2D(d);
      break;
     case 'e':
       char[][] e = {     {yes,yes,yes,yes,yes},
                          {yes,' ',' ',' ',' '},
                          {yes,yes,yes,' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,yes,yes,yes,yes}  };
       charMatrix = clone2D(e);
      break;
     case 'f':
       char[][] f = {     {yes,yes,yes,yes,yes},
                          {yes,' ',' ',' ',' '},
                          {yes,yes,yes,' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '}  };
       charMatrix = clone2D(f);
      break;
     case 'g':
       char[][] g = {     {' ',' ',yes,yes,yes},
                          {' ',yes,' ',' ',' '},
                          {yes,' ',' ',yes,yes},
                          {yes,' ',' ',' ',yes},
                          {' ',yes,yes,yes,' '}  };
       charMatrix = clone2D(g);
      break;
     case 'h':
       char[][] h = {     {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,yes,yes,yes,yes},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes}  };
       charMatrix = clone2D(h);
      break;
     case 'i':
       char[][] i = {     {yes,yes,yes,yes,yes},
                          {' ',' ',yes,' ',' '},
                          {' ',' ',yes,' ',' '},
                          {' ',' ',yes,' ',' '},
                          {yes,yes,yes,yes,yes}  };
       charMatrix = clone2D(i);
      break;
     case 'j':
       char[][] j = {     {' ',' ',' ',' ',yes},
                          {' ',' ',' ',' ',yes},
                          {' ',' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,yes,yes,yes,yes}  };
       charMatrix = clone2D(j);
      break;
     case 'k':
       char[][] k = {     {yes,' ',' ',yes,' '},
                          {yes,' ',yes,' ',' '},
                          {yes,yes,' ',' ',' '},
                          {yes,' ',yes,' ',' '},
                          {yes,' ',' ',yes,yes}  };
       charMatrix = clone2D(k);
      break;
     case 'l':
       char[][] l = {     {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,yes,yes,yes,yes}  };
       charMatrix = clone2D(l);
      break;
     case 'm':
       char[][] m = {     {yes,' ',' ',' ',yes},
                          {yes,yes,' ',yes,yes},
                          {yes,' ',yes,' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes}  };
       charMatrix = clone2D(m);
      break;
     case 'n':
       char[][] n = {     {yes,' ',' ',' ',yes},
                          {yes,yes,' ',' ',yes},
                          {yes,' ',yes,' ',yes},
                          {yes,' ',' ',yes,yes},
                          {yes,' ',' ',' ',yes}  };
       charMatrix = clone2D(n);
      break;
     case 'o':
       char[][] o = {     {' ',yes,yes,yes,' '},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {' ',yes,yes,yes,' '}  };
       charMatrix = clone2D(o);
      break;
     case 'p':
       char[][] p =   {   {yes,yes,yes,yes,' '},
                          {yes,' ',' ',' ',yes},
                          {yes,yes,yes,yes,' '},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '}  };
       charMatrix = clone2D(p);
      break;
     case 'q':
       char[][] q =   {   {yes,yes,yes,yes,' '},
                          {yes,' ',' ',yes,' '},
                          {yes,' ',' ',yes,' '},
                          {yes,' ',' ',yes,' '},
                          {yes,yes,yes,yes,yes}  };
       charMatrix = clone2D(q);
      break;
     case 'r':
       char[][] r =   {   {yes,yes,yes,yes,' '},
                          {yes,' ',' ',' ',yes},
                          {yes,yes,yes,yes,' '},
                          {yes,' ',yes,' ',' '},
                          {yes,' ',' ',yes,' '}  };
       charMatrix = clone2D(r);
      break;
     case 's':
       char[][] s =   {   {' ',yes,yes,yes,yes},
                          {yes,' ',' ',' ',' '},
                          {' ',yes,yes,yes,' '},
                          {' ',' ',' ',' ',yes},
                          {yes,yes,yes,yes,' '}  };
       charMatrix = clone2D(s);
      break;
     case 't':
       char[][] t =   {   {yes,yes,yes,yes,yes},
                          {' ',' ',yes,' ',' '},
                          {' ',' ',yes,' ',' '},
                          {' ',' ',yes,' ',' '},
                          {' ',' ',yes,' ',' '}  };
       charMatrix = clone2D(t);
      break;
     case 'u':
       char[][] u =   {   {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {' ',yes,yes,yes,' '}  };
       charMatrix = clone2D(u);
      break;
     case 'v':
       char[][] v =   {   {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {' ',yes,' ',yes,' '},
                          {' ',yes,' ',yes,' '},
                          {' ',' ',yes,' ',' '}  };
       charMatrix = clone2D(v);
      break;
     case 'w':
       char[][] w =   {   {yes,' ',' ',' ',yes},
                          {yes,' ',' ',' ',yes},
                          {yes,' ',yes,' ',yes},
                          {yes,' ',yes,' ',yes},
                          {' ',yes,yes,yes,' '}  };
       charMatrix = clone2D(w);
      break;
     case 'x':
       char[][] x =   {   {yes,' ',' ',' ',yes},
                          {' ',yes,' ',yes,' '},
                          {' ',' ',yes,' ',' '},
                          {' ',yes,' ',yes,' '},
                          {yes,' ',' ',' ',yes}  };
       charMatrix = clone2D(x);
      break;
     case '<':
       char[][] left ={   {' ',' ',yes,' ',yes},
                          {' ',yes,' ',yes,' '},
                          {yes,' ',yes,' ',' '},
                          {' ',yes,' ',yes,' '},
                          {' ',' ',yes,' ',yes} };
       charMatrix = clone2D(left);
      break;
     case '>':
       char[][] right ={   {yes,' ',yes,' ',' '},
                          {' ',yes,' ',yes,' '},
                          {' ',' ',yes,' ',yes},
                          {' ',yes,' ',yes,' '},
                          {yes,' ',yes,' ',' '} };
       charMatrix = clone2D(right);
      break;
     case '|':
       char[][] here ={   {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '},
                          {yes,' ',' ',' ',' '} };
       charMatrix = clone2D(here);
      break;
     case 'y':
       char[][] y =   {   {yes,' ',' ',' ',yes},
                          {' ',yes,' ',yes,' '},
                          {' ',' ',yes,' ',' '},
                          {' ',' ',yes,' ',' '},
                          {' ',' ',yes,' ',' '}  };
       charMatrix = clone2D(y);
      break;
     case 'z':
       char[][] z =   {   {yes,yes,yes,yes,yes},
                          {' ',' ',' ',yes,' '},
                          {' ',' ',yes,' ',' '},
                          {' ',yes,' ',' ',' '},
                          {yes,yes,yes,yes,yes}  };
       charMatrix = clone2D(z);
      break;
     default:
       char[][] def = {   {' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' '}  };
       charMatrix = clone2D(def);
      break;
    
  }
  
  //print2D(charMatrix);
  return charMatrix;
  
  
}

//cloning 2d arrays in java is weird
char[][] clone2D(char[][] input){
  char [][] clone = new char[input.length][];
  for (int i = 0; i < input.length; i++)
    clone[i] = input[i].clone();
    
   return clone;
    
}


void insertChar(char[][] injectMe, int startIndex){
  for (int i = 0; i < injectMe.length; i++)
   System.arraycopy(injectMe[i], 0, pixel[i], startIndex, injectMe[i].length); 
  
}


void marqueeText(String text){
  int offset = cols - (frameCount/marqSpeed % cols); //move along with cols 
  for (int i = 0; i < text.length(); i++) {  //run for each character in the message
    char[][] currentLetter = convertChar(text.charAt(i), 'b'); //convert the current char and store it in currentLetter
    int insertPos = offset + (currentLetter.length + textTracking) * i;

    if (insertPos > cols - currentLetter.length) { //check the end of the space of the new letter if it is available
      insertPos -= cols;
      //println("insert = " + insertPos);
    } 

    //clear the space between the letters
    for (int j = 0; j < textTracking; j++) {
      int clearCol = insertPos - 1 - j;
      if (clearCol >= 0) resetCol(insertPos - 1 - j);
    }
    
    //insert the letters if within the range
    if (insertPos >= 0) insertChar(currentLetter, insertPos);
  }
}

void displayText(String text){
 
  int offset = 0; //move along with cols 
  for (int i = 0; i < text.length(); i++) {  //run for each character in the message
    char[][] currentLetter = convertChar(text.charAt(i), 'b'); //convert the current char and store it in currentLetter
    int insertPos = offset + (currentLetter.length + textTracking) * i;

    if (insertPos > cols - currentLetter.length) { //check the end of the space of the new letter if it is available
      insertPos -= cols;
      //println("insert = " + insertPos);
    } 

    //clear the space between the letters
    for (int j = 0; j < textTracking; j++) {
      int clearCol = insertPos - 1 - j;
      if (clearCol >= 0) resetCol(insertPos - 1 - j);
    }
    
    //insert the letters if within the range
    if (insertPos >= 0) insertChar(currentLetter, insertPos);
  }

}
