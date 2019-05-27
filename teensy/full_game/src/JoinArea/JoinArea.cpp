
#include "Arduino.h"
#include "JoinArea.h"

JoinArea::JoinArea(){

	
}

void JoinArea::setup(int _x, ColorHolder * _col){
	center_x = _x;
	col = _col;

	col_width = 5;


	join_anim_time = 100;
	game_start_anim_time = 700;

	reset();
}

void JoinArea::reset(){
	anim_timer = 0;
	player_joined = false;
	doing_join_anim = false;
	doing_game_start_anim = false;
}

void JoinArea::update(int delta_millis, ColorHolder (&pixel)[NUM_COLS][NUM_ROWS]){

	anim_timer += delta_millis;

	if (doing_join_anim){
		update_join(delta_millis, pixel);
	}
	
	if (doing_game_start_anim && player_joined){
		update_game_start(delta_millis, pixel);
	}	
}

void JoinArea::update_join(int delta_millis, ColorHolder (&pixel)[NUM_COLS][NUM_ROWS]){
	int num_rows_lit = 0;

	if (doing_join_anim){
		float prc = (float)anim_timer/(float)join_anim_time;
		if (prc > 1)	prc = 1;
		num_rows_lit = NUM_ROWS * prc;
	}

	//error checking
	if (num_rows_lit > NUM_ROWS){
		num_rows_lit = NUM_ROWS;
	}

	//transfer it to the LEDs
	int start_x = center_x-2;
	for (int x=start_x; x<start_x+col_width; x++ ){
		for (int y=0; y<num_rows_lit; y++){
			pixel[x][y].set(col);
		}
	}
}

void JoinArea::update_game_start(int delta_millis, ColorHolder (&pixel)[NUM_COLS][NUM_ROWS]){

	float anim_prc = 1.0 - (float)anim_timer/(float)game_start_anim_time;
	float max_dist = 4 * anim_prc;
	float fade_range = 1.0;

	if (max_dist < -fade_range){
		max_dist = -fade_range;
	}

	

	int start_x = center_x-2;
	for (int x=start_x; x<start_x+col_width; x++ ){
		for (int y=0; y<NUM_ROWS; y++){

			float dist = abs((float)y - (float)player_y);

			float power = 1;
			if (dist > max_dist){
				power = 0;
			}
			float fade_point = max_dist-fade_range;
			if (dist < max_dist && dist > fade_point){
				float dist_prc = (dist-fade_point)/(max_dist-fade_point);
				power = dist_prc;
				power *= 0.5f;	//we need to really fade this thing to see any difference
			}

			if (power > 0){
				pixel[x][y].set(col, power);
			}
		}
	}


}

void JoinArea::mark_ready(){
	anim_timer = 0;
	doing_join_anim = true;
	player_joined = true;
}

void JoinArea::mark_game_start(int _player_y){
	anim_timer = 0;
	doing_join_anim = false;
	doing_game_start_anim = true;
	player_y = _player_y;
}
