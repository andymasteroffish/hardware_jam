/* obstacles can be any one of the following
  b - blocker
  s - shifter
  a - accelerator
  r - reverse       */

#ifndef Obstacle_h
#define Obstacle_h

#include "Arduino.h"
#include "ColorHolder/ColorHolder.h" 

class Obstacle
{
	public:
		#define NUM_COLS 96
		#define NUM_ROWS 5

		Obstacle();

		void setup(int _x, ColorHolder * b_col, ColorHolder * s_col, ColorHolder * a_col, ColorHolder * r_col);
		void set_action(char _action);
		void clear_rows();
		void shift();
		void update(int deltaMillis);

		int x;
  		char action;
  		bool on_rows[NUM_ROWS];

  		ColorHolder col;
  		ColorHolder * resting_col;



	private:

		//timing and animation
		int shift_timer;
		int shift_anim_time;

		//pointers to the basic colors the game uses. Don't mess with these
  		ColorHolder * blocker_col;
		ColorHolder * shifter_col;
		ColorHolder * accelerator_col;
		ColorHolder * reverse_col;


};

#endif