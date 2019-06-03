/* 
	Little light effect when a player dies
*/

#ifndef DeathEffect_h
#define DeathEffect_h

#include "Arduino.h"
#include "ColorHolder/ColorHolder.h" 

class DeathEffect
{
	public:
		#define NUM_COLS 96
		#define NUM_ROWS 5

		DeathEffect();
		void reset(int x, int y, float angle, ColorHolder * player_col);
		void update ();


		bool is_active;

		float x_pos, y_pos;
		int x_int, y_int;
		

  		ColorHolder col;
  		ColorHolder * resting_col;



	private:

		float x_vel;
		float y_vel;
		float grav;
		float x_fric;
		float y_fric;

		int frame_time;
		int next_update_time;

		int fade_time;
		int fade_timer;


};

#endif