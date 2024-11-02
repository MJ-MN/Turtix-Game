#ifndef __MOVEMENT_HPP
#define __MOVEMENT_HPP

#include "manual.hpp"
#include "map.hpp"

#define X_ACCELERATION 1000.0f
#define X_DECELERATION (4.0f * X_ACCELERATION)
#define Y_ACCELERATION 2000.0f
#define INITIAL_SPEED 0.1f
#define MAX_X_SPEED 800.0f
#define MAX_Y_SPEED 1000.0f
#define SPEED_STEP (X_DECELERATION * LOOP_TIME_MS / ONE_SECOND_MS)

class Movement {
protected:
	float* r;
	float v, a;

public:
	Movement(float* _r, float _v, float _a);
	Movement();
	virtual ~Movement();
	float get_r(void) const;
	float get_v(void) const;
	void set_r(float _r);
	void set_v(float _v);
	void set_a(float _a);
	float calc_r(float dt);
	void calc_v(float dt);
	void stop(void);
};

class MovementX : public Movement {
private:
public:
	MovementX(float* _r, float _v, float _a);
	MovementX();
	~MovementX();
	void calc_v(float penalty, float dt);
};

class MovementY : public Movement {
private:
public:
	MovementY(float* _r, float _v, float _a);
	MovementY();
	~MovementY();
	void calc_v(float penalty, float dt);
};

#endif /* __MOVEMENT_HPP */