struct Button{
  int pin;
  int next_check_time;  //for debounce
  bool is_held;
};

#define NUM_BUTTONS 1
Button buttons[NUM_BUTTONS];

int debounce_time = 30; //millis

void setup() {

  buttons[0].pin = 2;
  for (int i=0; i<NUM_BUTTONS; i++){
    buttons[i].next_check_time = 0;
    buttons[i].is_held = false;
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i=0; i<NUM_BUTTONS; i++){
    if (millis() > buttons[i].next_check_time){
      //get the current value
      bool cur_val = false;
      if (digitalRead(buttons[i].pin) == LOW){
        cur_val = true;
      }
  
      //is that different form the current value?
      if (cur_val != buttons[i].is_held){
        buttons[i].is_held = cur_val;
        buttons[i].next_check_time = millis() + debounce_time;
        Serial.println("button: "+String(buttons[i].is_held));
        if (buttons[i].is_held){
          Keyboard.press(KEY_A);
        }else{
          Keyboard.release(KEY_A);
        }
      }
      
    }
  }

}
