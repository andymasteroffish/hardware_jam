
#include "Arduino.h"
#include "ColorHolder.h"

ColorHolder::ColorHolder(){
	set(0,0,0);
	has_been_changed = true;
}

void ColorHolder::set(int _r,int _g, int _b){
	if (r != _r || g != _g || b != _b){
		has_been_changed = true;
	}
	r = _r ;
	g = _g ;
	b = _b ;
}

void ColorHolder::set(int _r,int _g, int _b, float fade_prc){
	int new_r = _r * fade_prc;
	int new_g = _g * fade_prc;
	int new_b = _b * fade_prc;

	if (r != new_r || g != new_g || b != new_b){
		has_been_changed = true;
	}

	r = new_r;
	g = new_g;
	b = new_b;
}

void ColorHolder::set(ColorHolder source){
	set(source.r, source.g, source.b);
}
void ColorHolder::set(ColorHolder source, float fade_prc){
	set(source.r, source.g, source.b, fade_prc);
}

uint32_t ColorHolder::get_uint(){
	return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
uint32_t ColorHolder::get_uint(float fade_prc){
	int new_r = r * fade_prc;
	int new_g = g * fade_prc;
	int new_b = b * fade_prc;
	return ((uint32_t)new_r << 16) | ((uint32_t)new_g << 8) | new_b;
}

uint32_t ColorHolder::get_uint_dotstar(){
	return ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;
}
uint32_t ColorHolder::get_uint_dotstar(float fade_prc){
	int new_r = r * fade_prc;
	int new_g = g * fade_prc;
	int new_b = b * fade_prc;
	return ((uint32_t)new_g << 16) | ((uint32_t)new_r << 8) | new_b;
}
