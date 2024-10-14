#include "../inc/turtix.hpp"
#include "../inc/movement.hpp"

using namespace std;

Turtix::Turtix(sf::RenderWindow* _window, Map* _map)
{
	if (!this->turtixTexture.loadFromFile(TURTIX_IMAGE_FILE)) {
		cerr << "Failed to load " << TURTIX_IMAGE_FILE << "!" << endl;
	}
	this->turtixSprite.setTexture(this->turtixTexture);
	this->turtixSprite.setScale(sf::Vector2f(TURTIX_SCALE, TURTIX_SCALE));
	this->window = _window;
	sf::Vector2f turtixPos = {
		(WIDTH_FRAMES / 2.0f * SCALED_FRAME_SIZE - TURTIX_SCALE * turtixTexture.getSize().x / 2.0f),
		(HEIGHT_FRAMES / 2.0f * SCALED_FRAME_SIZE - TURTIX_SCALE * turtixTexture.getSize().y / 2.0f)
	};
	this->movement_x = new MovementX(_map, turtixPos.x, 0.0f, X_ACCELERATION);
	this->movement_y = new MovementY(_map, turtixPos.y, 0.0f, Y_ACCELERATION);
}

Turtix::~Turtix()
{
	delete this->movement_x;
	delete this->movement_y;
}

sf::FloatRect Turtix::get_rect(void) const
{
	return this->turtixSprite.getGlobalBounds();
}

sf::Vector2f Turtix::get_center_pos(void) const
{
	sf::FloatRect turtixRect(this->get_rect());
	return sf::Vector2f(
		turtixRect.left + turtixRect.width / 2.0f,
		turtixRect.top + turtixRect.height / 2.0f
	);
}

void Turtix::draw(void)
{
	this->turtixSprite.setPosition(sf::Vector2f(this->movement_x->get_r(), this->movement_y->get_r()));
	this->window->draw(this->turtixSprite);
}

void Turtix::move(const Key* key, float dt)
{
	sf::FloatRect turtixRect = this->get_rect();

	this->move_x(turtixRect, key, dt);
	this->move_y(turtixRect, key, dt);
}

void Turtix::move_x(sf::FloatRect& turtixRect, const Key* key, float dt)
{
	float penalty = 0.0f;

	if (key->right_pressed) {
		if (this->movement_x->get_v() == 0.0f) {
			this->movement_x->set_v(INITIAL_SPEED);
		} else if (this->movement_x->get_v() > 0.0f) {
			this->movement_x->set_a(X_ACCELERATION);
		} else {
			this->movement_x->set_a(X_DECELERATION);
		}
	} else if (key->left_pressed) {
		if (this->movement_x->get_v() ==  0.0f) {
			this->movement_x->set_v(-INITIAL_SPEED);
		} else if (this->movement_x->get_v() < 0.0f) {
			this->movement_x->set_a(-X_ACCELERATION);
		} else {
			this->movement_x->set_a(-X_DECELERATION);
		}
	} else if (!key->right_pressed && !key->left_pressed) {
		if (this->movement_x->get_v() > SPEED_STEP) {
			this->movement_x->set_a(-X_DECELERATION);
		} else if (this->movement_x->get_v() < -SPEED_STEP) {
			this->movement_x->set_a(X_DECELERATION);
		} else {
			this->movement_x->set_v(0.0f);
			this->movement_x->set_a(0.0f);
		}
	}
	this->movement_x->calc_x(turtixRect, dt);
}

void Turtix::move_y(sf::FloatRect& turtixRect, const Key* key, float dt)
{
	float penalty = 0.0f;

	if (key->up_pressed && !key->last_up_pressed) {
		this->movement_y->set_v(-JUMP_SPEED);
	}
	this->movement_y->calc_y(turtixRect, dt);
}
