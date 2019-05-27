/* 
	Helper class for dealing with color
*/

#ifndef ColorHolder_h
#define ColorHolder_h

#include "Arduino.h"

class ColorHolder
{
	public:
		ColorHolder();

		void set(int _r,int _g, int _b);
		void set(int _r,int _g, int _b, float fade_prc);
		void set(ColorHolder * source);
		void set(ColorHolder * source, float fade_prc);
		void blank();

		uint32_t get_uint();
		uint32_t get_uint(float fade_prc);

		uint32_t get_uint_dotstar();
		uint32_t get_uint_dotstar(float fade_prc);

		int r;
		int g;
		int b;

		bool has_been_changed;


	private:

};

#endif