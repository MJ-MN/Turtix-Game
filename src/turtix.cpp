#include "../inc/turtix.hpp"
#include "../inc/movement.hpp"

using namespace std;

Turtix::Turtix(sf::RenderWindow* _window, Map* _map)
{
	this->turtixVertices.setPrimitiveType(sf::Triangles);
	this->turtixVertices.resize(POINTS_IN_FRAME);
	this->load_texture();
	this->window = _window;
	sf::Vector2f turtixPos = {
		(MAP_WIDTH_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE - TURTIX_SCALED_FRAME_SIZE / 2.0f),
		(MAP_HEIGHT_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE - TURTIX_SCALED_FRAME_SIZE / 2.0f)
	};
	this->movement_x = new MovementX(_map, turtixPos.x, 0.0f, X_ACCELERATION);
	this->movement_y = new MovementY(_map, turtixPos.y, 0.0f, Y_ACCELERATION);
	this->init_frame_sprite();
}

Turtix::~Turtix()
{
	delete this->movement_x;
	delete this->movement_y;
}

void Turtix::load_texture(void)
{
	if (!this->turtixTextureLeft.loadFromFile(TURTIX_LEFT_IMAGE_FILE)) {
		cerr << "Failed to load " << TURTIX_LEFT_IMAGE_FILE << "!" << endl;
	}
	if (!this->turtixTextureRight.loadFromFile(TURTIX_RIGHT_IMAGE_FILE)) {
		cerr << "Failed to load " << TURTIX_RIGHT_IMAGE_FILE << "!" << endl;
	}
}

void Turtix::init_frame_sprite(void)
{
	this->turtixVertices[0].texCoords = sf::Vector2f(0.0f, 0.0f);
	this->turtixVertices[1].texCoords = this->turtixVertices[4].texCoords = sf::Vector2f(0.0f, TURTIX_FRAME_SIZE);
	this->turtixVertices[2].texCoords = this->turtixVertices[3].texCoords = sf::Vector2f(TURTIX_FRAME_SIZE, 0.0f);
	this->turtixVertices[5].texCoords = sf::Vector2f(TURTIX_FRAME_SIZE, TURTIX_FRAME_SIZE);
}

sf::FloatRect Turtix::get_rect(void) const
{
	sf::Vector2f center_pos = this->get_center_pos();

	return sf::FloatRect(
		center_pos.x - TURTIX_FRAME_SIZE_EFFECTIVE / 2.0f, center_pos.y - TURTIX_FRAME_SIZE_EFFECTIVE / 2.0f,
		TURTIX_FRAME_SIZE_EFFECTIVE, TURTIX_FRAME_SIZE_EFFECTIVE
	);
}

void Turtix::update_frame_sprite(void)
{
	static int frame_number = 0;

	if (this->movement_x->get_v() > INITIAL_SPEED ||
		this->movement_x->get_v() < -INITIAL_SPEED) {
		frame_number++;
		if (frame_number >= TURTIX_IMAGE_FRAMES) {
			frame_number = 0;
		}
	} else {
		frame_number = 0;
	}
	sf::Vector2i tile_frame_pos(frame_number % TURTIX_IMAGE_WIDTH_FRAMES, frame_number / TURTIX_IMAGE_WIDTH_FRAMES);
	this->turtixVertices[0].texCoords = sf::Vector2f(
		tile_frame_pos.x * TURTIX_IMAGE_FRAME_SIZE,
		tile_frame_pos.y * TURTIX_IMAGE_FRAME_SIZE
	);
	this->turtixVertices[1].texCoords = this->turtixVertices[4].texCoords = this->turtixVertices[0].texCoords +
		sf::Vector2f(0.0f, TURTIX_FRAME_SIZE);
	this->turtixVertices[2].texCoords = this->turtixVertices[3].texCoords = this->turtixVertices[0].texCoords +
		sf::Vector2f(TURTIX_FRAME_SIZE, 0.0f);
	this->turtixVertices[5].texCoords = this->turtixVertices[0].texCoords +
		sf::Vector2f(TURTIX_FRAME_SIZE, TURTIX_FRAME_SIZE);
}

sf::Vector2f Turtix::get_center_pos(void) const
{
	return (this->turtixVertices[0].position + this->turtixVertices[5].position) / 2.0f;
}

void Turtix::move(const Key& key, float dt)
{
	sf::FloatRect turtixRect = this->get_rect();

	this->move_x(turtixRect, key, dt);
	this->move_y(turtixRect, key, dt);
	this->turtixVertices[0].position = sf::Vector2f(turtixRect.left - TRUTIX_FRAME_MARGIN / 2.0f, turtixRect.top - TRUTIX_FRAME_MARGIN / 2.0f);
	this->turtixVertices[1].position = this->turtixVertices[4].position = this->turtixVertices[0].position + sf::Vector2f(0.0f, TURTIX_SCALED_FRAME_SIZE);
	this->turtixVertices[2].position = this->turtixVertices[3].position = this->turtixVertices[0].position + sf::Vector2f(TURTIX_SCALED_FRAME_SIZE, 0.0f);
	this->turtixVertices[5].position = this->turtixVertices[0].position + sf::Vector2f(TURTIX_SCALED_FRAME_SIZE, TURTIX_SCALED_FRAME_SIZE);
}

void Turtix::move_x(sf::FloatRect& turtixRect, const Key& key, float dt)
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
			this->movement_x->set_v(0.0f);
			this->movement_x->set_a(0.0f);
		}
	}
	this->movement_x->calc_x(turtixRect, dt);
}

void Turtix::move_y(sf::FloatRect& turtixRect, const Key& key, float dt)
{
	if (key.up_pressed && !key.last_up_pressed && this->movement_y->is_on_the_ground()) {
		this->movement_y->set_v(-JUMP_SPEED);
	}
	this->movement_y->calc_y(turtixRect, dt);
}

void Turtix::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->direction == Direction::LEFT) {
		states.texture = &this->turtixTextureLeft;
	} else {
		states.texture = &this->turtixTextureRight;
	}
	target.draw(this->turtixVertices, states);
}
