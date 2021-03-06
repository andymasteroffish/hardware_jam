
#include "Arduino.h"
#include "DeathEffect.h"

DeathEffect::DeathEffect(){
	grav = 0.006;
	x_fric = 0.98;
	y_fric = 1;//0.99;

	frame_time = 10;
	fade_time = 600;

	is_active = false;
}

void DeathEffect::reset(int x, int y, float angle, ColorHolder * player_col){
	x_pos = (float)x + 0.5;
	y_pos = (float)y + 0.5;
	x_int = x;
	y_int = y;

	resting_col = player_col;

	float starting_force = 0.15;
	x_vel = cos(angle) * starting_force;
	y_vel = sin(angle) * starting_force;

	y_vel -= starting_force;

	next_update_time = millis();
	is_active = true;
	fade_timer = 0;

	Serial.print("set me ");
	Serial.println(angle);
	Serial.print("  my y ");
	Serial.println(y_vel);
}

void DeathEffect::update (){

	if (millis() < next_update_time){
		return;
	}
	next_update_time += frame_time;
	fade_timer += frame_time;

	


	y_vel += grav;

	x_vel *= x_fric;
	y_vel *= y_fric;

	x_pos += x_vel;
	y_pos += y_vel;

	if (x_pos < 0){
		x_pos += NUM_COLS;
	}

	x_int = round(x_pos);
	y_int = round(y_pos);

	float fade_prc = 1.0 - (float)fade_timer/(float)fade_time;
	fade_prc = powf(fade_prc, 3);

	col.set(resting_col, fade_prc);

	Serial.print("update me ");
	Serial.println(fade_timer);
	Serial.print("  x ");
	Serial.print(x_int);
	Serial.print("  y ");
	Serial.println(y_int);


	if (fade_timer > fade_time){
		is_active = false;
		Serial.println(" time out im ded");
	}
	if (y_int >= NUM_ROWS){
		is_active = false;
		Serial.println(" off screen im ded");
	}

}


