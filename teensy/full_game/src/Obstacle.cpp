
#include "Arduino.h"
#include "Obstacle.h"

Obstacle::Obstacle(){
	
}

void Obstacle::setup(int _x, ColorHolder * b_col, ColorHolder * s_col, ColorHolder * a_col, ColorHolder * r_col){
	
	shift_anim_time = 100;

	x = _x;
	
	blocker_col = b_col;;
	shifter_col = s_col;
	accelerator_col = a_col;
	reverse_col = r_col; 
	
	set_action('b');

	clear_rows();
}

void Obstacle::set_action(char _action){
	action = _action;
	if (action == 'b')	resting_col = blocker_col;
	if (action == 's')	resting_col = shifter_col;
	if (action == 'a')	resting_col = accelerator_col;
	if (action == 'r')	resting_col = reverse_col;

	col.set(resting_col);

	clear_rows();

	on_rows[0] = true;
    on_rows[1] = true;

    shift_timer = 999;

}

void Obstacle::clear_rows(){
	for (int y=0; y<NUM_ROWS; y++){
		on_rows[y] = false;
	}
}

void Obstacle::shift(){
	bool temp = on_rows[NUM_ROWS - 1];
  	for (int r = NUM_ROWS - 1; r > 0; r--) {
    	on_rows[r] = on_rows[r - 1];
  	}
  	on_rows[0] = temp;

  	shift_timer = 0;
}

void Obstacle::update(int deltaMillis){
	shift_timer += deltaMillis;


	if (shift_timer > shift_anim_time){
		col.set(resting_col);
	}else{
		float prc = (float)shift_timer / (float)shift_anim_time;
		prc = powf(prc, 3);	//keep it low

		int alt_r = 175;
		int alt_g = 175;
		int alt_b = 175;


		int new_r = resting_col->r * prc + alt_r * (1.0-prc);
		int new_g = resting_col->g * prc + alt_g * (1.0-prc);
		int new_b = resting_col->b * prc + alt_b * (1.0-prc);

		col.set(new_r, new_g, new_b);
	}
}

