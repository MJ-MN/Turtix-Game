#ifndef __MANUAL_HPP
#define __MANUAL_HPP

#define ONE_SECOND_MS 1000.0f
#define LOOP_TIME_MS 50

enum Direction {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NONE
};

struct Key {
	bool up_pressed;
	bool last_up_pressed;
	bool right_pressed;
	bool left_pressed;
};

struct Position {
	int col;
	int row;
};

#endif /* __MANUAL_HPP */
