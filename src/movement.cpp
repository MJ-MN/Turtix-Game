#include "../inc/movement.hpp"
#include "../inc/map.hpp"

Movement::Movement(float _r, float _v, float _a)
{
	this->r = _r;
	this->v = _v;
	this->a = _a;
}

Movement::~Movement()
{

}

float Movement::get_r(void) const
{
	return this->r;
}

float Movement::get_v(void) const
{
	return this->v;
}

void Movement::set_r(float _r)
{
	this->r = _r;
}

void Movement::set_v(float _v)
{
	this->v = _v;
}

void Movement::set_a(float _a)
{
	this->a = _a;
}

void Movement::calc_r(float dt)
{
	this->r = 0.5 * this->a * (dt / ONE_SECOND_MS) * (dt / ONE_SECOND_MS) + this->v * (dt / ONE_SECOND_MS) + this->r;
}

void Movement::calc_v(float dt)
{
	this->v = this->a * (dt / ONE_SECOND_MS) + this->v;
	if (this->v < -MAX_SPEED) {
		this->v = -MAX_SPEED;
	} else if (this->v > MAX_SPEED) {
		this->v = MAX_SPEED;
	}
}

void Movement::check_v(float penalty, float dt)
{
	if (penalty == 0.0f) {
		this->calc_v(dt);
	} else if (penalty > 0.0f) {
		this->v = -INITIAL_SPEED;
	} else {
		this->v = INITIAL_SPEED;
	}
}

MovementX::MovementX(Map* _map, float _r, float _v, float _a)
	: Movement(_r, _v, _a)
{
	this->map = _map;
}

MovementX::~MovementX()
{

}

void MovementX::calc_x(sf::FloatRect& rect, float dt)
{
	float penalty = 0.0f;

	Movement::calc_r(dt);
	rect.left = this->r;
	penalty = this->map->is_valid_x(rect, this->v);
	this->r += penalty;
	rect.left = this->r;
	this->check_v(penalty, dt);
}

MovementY::MovementY(Map* _map, float _r, float _v, float _a)
	: Movement(_r, _v, _a)
{
	this->map = _map;
}

MovementY::~MovementY()
{
	
}

void MovementY::calc_y(sf::FloatRect& rect, float dt)
{
	float penalty = 0.0f;

	Movement::calc_r(dt);
	rect.top = this->r;
	penalty = this->map->is_valid_y(rect, this->v);
	this->r += penalty;
	rect.top = this->r;
	this->check_v(penalty, dt);
}
