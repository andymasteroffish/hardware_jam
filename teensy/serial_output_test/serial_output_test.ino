// For testing the Processing sketch serial_display

int x = 0;
int y = 0;

char colors[4];
int cur_col = 0;

void setup() {
  Serial.begin(9600);

  colors[0] = 'a';
  colors[1] = 'b';
  colors[2] = 'r';
  colors[3] = '-';
}

void loop() {
  // put your main code here, to run repeatedly:

  char color = 'a';

  String line = String(x) + "," + String(y) + "," + colors[cur_col] + "\n";
  Serial.print(line);

  x++;
  if (x >= 85){
    x=0;
    y++;
    if (y >= 5){
      y=0;
    }
  }

  cur_col++;
  if (cur_col >= 4){
    cur_col = 0;
  }

  delay(400);

}
