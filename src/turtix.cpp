#include "../inc/turtix.hpp"
#include "../inc/movement.hpp"

using namespace std;

Turtix::Turtix(sf::RenderWindow* _window, Map* _map)
{
	sf::Vector2f init_pos = {
		(MAP_WIDTH_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE - TURTIX_SCALED_FRAME_SIZE / 2.0f),
		(MAP_HEIGHT_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE - TURTIX_SCALED_FRAME_SIZE / 2.0f)
	};

	this->vertices.setPrimitiveType(sf::Triangles);
	this->vertices.resize(POINTS_IN_FRAME);
	this->load_texture();
	this->window = _window;
	this->map = _map;
	this->set_pos(init_pos);
	this->movement_x = new MovementX(&this->vertices[0].position.x, 0.0f, X_ACCELERATION);
	this->movement_y = new MovementY(&this->vertices[0].position.y, 0.0f, Y_ACCELERATION);
	this->set_frame_sprite(sf::Vector2i(0, 0));
	this->direction = Direction::RIGHT;
	this->on_the_ground = false;
	this->on_the_ladder = false;
}

Turtix::Turtix()
{

}

Turtix::~Turtix()
{
	delete this->movement_x;
	delete this->movement_y;
}

Turtix::Turtix(const Turtix& turtix)
{
	this->direction = turtix.direction;
	this->vertices = turtix.vertices;
	this->textureLeft = turtix.textureLeft;
	this->textureRight = turtix.textureRight;
	this->textureUp = turtix.textureUp;
	this->window = turtix.window;
	this->movement_x = new MovementX();
	this->movement_x = turtix.movement_x;
	this->movement_y = new MovementY();
	this->movement_y = turtix.movement_y;
}

Turtix& Turtix::operator=(const Turtix& turtix)
{
	this->direction = turtix.direction;
	this->vertices = turtix.vertices;
	this->textureLeft = turtix.textureLeft;
	this->textureRight = turtix.textureRight;
	this->textureUp = turtix.textureUp;
	this->window = turtix.window;
	*this->movement_x = *turtix.movement_x;
	*this->movement_y = *turtix.movement_y;
	return *this;
}

void Turtix::load_texture(void)
{
	if (!this->textureLeft.loadFromFile(TURTIX_LEFT_IMAGE_FILE)) {
		cerr << "Failed to load " << TURTIX_LEFT_IMAGE_FILE << "!" << endl;
	}
	if (!this->textureRight.loadFromFile(TURTIX_RIGHT_IMAGE_FILE)) {
		cerr << "Failed to load " << TURTIX_RIGHT_IMAGE_FILE << "!" << endl;
	}
	if (!this->textureUp.loadFromFile(TURTIX_UP_IMAGE_FILE)) {
		cerr << "Failed to load " << TURTIX_UP_IMAGE_FILE << "!" << endl;
	}
}

void Turtix::set_pos(const sf::Vector2f& pos)
{
	this->vertices[0].position = pos;
	this->vertices[1].position = this->vertices[4].position = this->vertices[0].position + sf::Vector2f(0.0f, TURTIX_SCALED_FRAME_SIZE);
	this->vertices[2].position = this->vertices[3].position = this->vertices[0].position + sf::Vector2f(TURTIX_SCALED_FRAME_SIZE, 0.0f);
	this->vertices[5].position = this->vertices[0].position + sf::Vector2f(TURTIX_SCALED_FRAME_SIZE, TURTIX_SCALED_FRAME_SIZE);
}

void Turtix::set_frame_sprite(const sf::Vector2i& frame_pos)
{
	this->vertices[0].texCoords = sf::Vector2f(
		frame_pos.x * TURTIX_IMAGE_FRAME_SIZE,
		frame_pos.y * TURTIX_IMAGE_FRAME_SIZE
	);
	this->vertices[1].texCoords = this->vertices[4].texCoords = this->vertices[0].texCoords +
		sf::Vector2f(0.0f, TURTIX_IMAGE_FRAME_SIZE);
	this->vertices[2].texCoords = this->vertices[3].texCoords = this->vertices[0].texCoords +
		sf::Vector2f(TURTIX_IMAGE_FRAME_SIZE, 0.0f);
	this->vertices[5].texCoords = this->vertices[0].texCoords +
		sf::Vector2f(TURTIX_IMAGE_FRAME_SIZE, TURTIX_IMAGE_FRAME_SIZE);
}

sf::FloatRect Turtix::get_rect(void) const
{
	sf::Vector2f center_pos = this->get_center_pos();

	return sf::FloatRect(
		center_pos.x - TURTIX_SCALED_FRAME_SIZE_EFF / 2.0f, center_pos.y - TURTIX_SCALED_FRAME_SIZE_EFF / 2.0f,
		TURTIX_SCALED_FRAME_SIZE_EFF, TURTIX_SCALED_FRAME_SIZE_EFF
	);
}

void Turtix::update_frame_sprite(void)
{
	static int frame_number = 0;

	if ((!this->on_the_ladder && X_MOVING) ||
		(this->on_the_ladder && Y_MOVING)) {
		frame_number++;
		if (frame_number >= TURTIX_IMAGE_FRAMES) {
			frame_number = 0;
		}
	} else {
		frame_number = 0;
	}
	this->set_frame_sprite(sf::Vector2i(frame_number % TURTIX_IMAGE_WIDTH_FRAMES, frame_number / TURTIX_IMAGE_WIDTH_FRAMES));
}

sf::Vector2f Turtix::get_center_pos(void) const
{
	return (this->vertices[0].position + this->vertices[5].position) / 2.0f;
}

void Turtix::move(const Key& key, float dt)
{
	sf::FloatRect turtixRect = this->get_rect();

	this->move_x(key);
	this->update_x(turtixRect, dt);
	this->move_y(key);
	this->update_y(turtixRect, dt);
	if (this->map->is_on_the_ladder(turtixRect)) {
		this->on_the_ladder = true;
	} else {
		this->on_the_ladder = false;
	}
	this->set_pos(sf::Vector2f(turtixRect.left - TRUTIX_SCALED_FRAME_MARGIN / 2.0f, turtixRect.top - TRUTIX_SCALED_FRAME_MARGIN / 2.0f));
}

void Turtix::move_x(const Key& key)
{
	if (key.right_pressed) {
		if (this->movement_x->get_v() == 0.0f) {
			this->movement_x->set_v(INITIAL_SPEED);
			this->direction = Direction::RIGHT;
		} else if (this->movement_x->get_v() > 0.0f) {
			this->movement_x->set_a(X_ACCELERATION);
			this->direction = Direction::RIGHT;
		} else {
			this->movement_x->set_a(X_DECELERATION);
		}
	} else if (key.left_pressed) {
		if (this->movement_x->get_v() ==  0.0f) {
			this->movement_x->set_v(-INITIAL_SPEED);
			this->direction = Direction::LEFT;
		} else if (this->movement_x->get_v() < 0.0f) {
			this->movement_x->set_a(-X_ACCELERATION);
			this->direction = Direction::LEFT;
		} else {
			this->movement_x->set_a(-X_DECELERATION);
		}
	} else if (!key.right_pressed && !key.left_pressed) {
		if (this->movement_x->get_v() > SPEED_STEP) {
			this->movement_x->set_a(-X_DECELERATION);
		} else if (this->movement_x->get_v() < -SPEED_STEP) {
			this->movement_x->set_a(X_DECELERATION);
		} else {
			this->movement_x->stop();
		}
	}
}

void Turtix::update_x(sf::FloatRect& turtixRect, float dt)
{
	float penalty_x = 0.0f;

	turtixRect.left += this->movement_x->calc_r(dt);
	penalty_x = this->map->is_valid_x(turtixRect, this->movement_x->get_v());
	turtixRect.left += penalty_x;
	this->movement_x->calc_v(penalty_x, dt);
}

void Turtix::move_y(const Key& key)
{
	this->movement_y->set_a(Y_ACCELERATION);
	if (this->on_the_ladder) {
		this->movement_y->stop();
		if (key.up_pressed) {
			this->movement_y->set_v(-0.2f * MAX_Y_SPEED);
		} else if (key.down_pressed) {
			this->movement_y->set_v(0.2f * MAX_Y_SPEED);
		}
	} else if (key.down_pressed) {

	} else if (key.up_pressed && !key.last_up_pressed && this->on_the_ground) {
		this->movement_y->set_v(-MAX_Y_SPEED);
	}
}

void Turtix::update_y(sf::FloatRect& rect, float dt)
{
	float penalty_y = 0.0f;

	rect.top += this->movement_y->calc_r(dt);
	penalty_y = this->map->is_valid_y(rect, this->movement_y->get_v());
	rect.top += penalty_y;
	this->on_the_ground = (penalty_y < 0.0f) ? true : false;
	this->movement_y->calc_v(penalty_y, dt);
}

void Turtix::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->on_the_ladder) {
		states.texture = &this->textureUp;
	} else if (this->direction == Direction::LEFT) {
		states.texture = &this->textureLeft;
	} else {
		states.texture = &this->textureRight;
	}
	target.draw(this->vertices, states);
}
