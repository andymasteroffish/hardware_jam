

#ifndef JoinArea_h
#define JoinArea_h

#include "Arduino.h"
#include "ColorHolder/ColorHolder.h" 

class JoinArea
{
	public:
		#define NUM_COLS 96
		#define NUM_ROWS 5

		JoinArea();


		void setup(int _x, ColorHolder * _col);
		void reset();
		void update(int delta_millis, ColorHolder (&pixel)[NUM_COLS][NUM_ROWS]);
		void update_join(int delta_millis, ColorHolder (&pixel)[NUM_COLS][NUM_ROWS]);
		void update_game_start(int delta_millis, ColorHolder (&pixel)[NUM_COLS][NUM_ROWS]);

		void mark_ready();
		void mark_game_start(int _player_y);

		ColorHolder * col;
		int center_x;
		int col_width;

		int player_y;

		bool player_joined;

		float anim_timer;


		bool doing_join_anim;
		float join_anim_time;

		bool doing_game_start_anim;
		float game_start_anim_time;


	private:

};

#endif