#ifndef __MOVEMENT_HPP
#define __MOVEMENT_HPP

#include "manual.hpp"
#include "map.hpp"

#define X_ACCELERATION 1000.0f
#define X_DECELERATION (4.0f * X_ACCELERATION)
#define Y_ACCELERATION 3000.0f
#define INITIAL_SPEED 0.1f
#define JUMP_SPEED 800.0f
#define MAX_SPEED 800.0f
#define SPEED_STEP (X_DECELERATION * LOOP_TIME_MS / ONE_SECOND_MS)

class Movement {
protected:
	float r, v, a;

	void calc_r(float dt);
public:
	Movement(float _r, float _v, float _a);
	virtual ~Movement();
	float get_r(void) const;
	float get_v(void) const;
	void set_r(float _r);
	void set_v(float _v);
	void set_a(float _a);
	void calc_v(float dt);
	void check_v(float penalty, float dt);
};

class MovementX : public Movement {
private:
	Map* map;
public:
	MovementX(Map* _map, float _r, float _v, float _a);
	~MovementX();
	void calc_x(sf::FloatRect& rect, float dt);
};

class MovementY : public Movement {
private:
	Map* map;
public:
	MovementY(Map* _map, float _r, float _v, float _a);
	~MovementY();
	void calc_y(sf::FloatRect& rect, float dt);
};

#endif /* __MOVEMENT_HPP */