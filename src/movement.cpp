#include "../inc/movement.hpp"
#include "../inc/map.hpp"

Movement::Movement(float* _r, float _v, float _a)
{
	this->r = _r;
	this->v = _v;
	this->a = _a;
}

Movement::Movement()
{

}

Movement::~Movement()
{

}

float Movement::get_r(void) const
{
	return *this->r;
}

float Movement::get_v(void) const
{
	return this->v;
}

void Movement::set_r(float _r)
{
	*this->r = _r;
}

void Movement::set_v(float _v)
{
	this->v = _v;
}

void Movement::set_a(float _a)
{
	this->a = _a;
}

float Movement::calc_r(float dt)
{
	float dr = 0.5 * this->a * (dt / ONE_SECOND_MS) * (dt / ONE_SECOND_MS) + this->v * (dt / ONE_SECOND_MS);

	*this->r += dr;
	return dr;
}

void Movement::stop(void)
{
	this->v = 0.0f;
	this->a = 0.0f;
}

void Movement::calc_v(float dt)
{
	this->v = this->a * (dt / ONE_SECOND_MS) + this->v;
}

MovementX::MovementX(float* _r, float _v, float _a)
	: Movement(_r, _v, _a)
{

}

MovementX::MovementX()
{

}

MovementX::~MovementX()
{

}

void MovementX::calc_v(float penalty, float dt)
{
	Movement::calc_v(dt);
	if (penalty != 0.0f) {
		if (penalty > 0.0f) {
			this->v = -INITIAL_SPEED;
		} else {
			this->v = INITIAL_SPEED;
		}
	}
	if (this->v < -MAX_X_SPEED) {
		this->v = -MAX_X_SPEED;
	} else if (this->v > MAX_X_SPEED) {
		this->v = MAX_X_SPEED;
	}
}

MovementY::MovementY(float* _r, float _v, float _a)
	: Movement(_r, _v, _a)
{

}

MovementY::MovementY()
{

}

MovementY::~MovementY()
{
	
}

void MovementY::calc_v(float penalty, float dt)
{
	Movement::calc_v(dt);
	if (penalty != 0.0f) {
		if (penalty > 0.0f) {
			this->v = -INITIAL_SPEED;
		} else {
			this->v = INITIAL_SPEED;
		}
	}
	if (this->v < -MAX_Y_SPEED) {
		this->v = -MAX_Y_SPEED;
	} else if (this->v > MAX_Y_SPEED) {
		this->v = MAX_Y_SPEED;
	}
}
