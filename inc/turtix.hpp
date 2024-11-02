#ifndef __TURTIX_HPP
#define __TURTIX_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "map.hpp"
#include "movement.hpp"

#define TURTIX_IMAGE_FRAMES 16
#define TURTIX_IMAGE_WIDTH_FRAMES 5
#define TURTIX_IMAGE_FRAME_SIZE 95
#define TRUTIX_FRAME_MARGIN 25
#define TURTIX_FRAME_SIZE_EFF (TURTIX_IMAGE_FRAME_SIZE - TRUTIX_FRAME_MARGIN)
#define TURTIX_SCALE 1.5f
#define TRUTIX_SCALED_FRAME_MARGIN (TURTIX_SCALE * TRUTIX_FRAME_MARGIN)
#define TURTIX_SCALED_FRAME_SIZE (TURTIX_SCALE * TURTIX_IMAGE_FRAME_SIZE)
#define TURTIX_SCALED_FRAME_SIZE_EFF (TURTIX_SCALE * TURTIX_FRAME_SIZE_EFF)

#define X_MOVING (this->movement_x->get_v() > INITIAL_SPEED || \
	this->movement_x->get_v() < -INITIAL_SPEED)
#define Y_MOVING (this->movement_y->get_v() > INITIAL_SPEED || \
	this->movement_y->get_v() < -INITIAL_SPEED)

const std::string TURTIX_LEFT_IMAGE_FILE = "./Sprite/turtix_left.png";
const std::string TURTIX_RIGHT_IMAGE_FILE = "./Sprite/turtix_right.png";
const std::string TURTIX_UP_IMAGE_FILE = "./Sprite/turtix_up.png";

class Turtix : public sf::Drawable {
private:
	Direction direction;
	bool on_the_ground;
	bool on_the_ladder;
	sf::VertexArray vertices;
	sf::Texture textureLeft;
	sf::Texture textureRight;
	sf::Texture textureUp;
	sf::RenderWindow* window;
	Map* map;
	MovementX* movement_x;
	MovementY* movement_y;

	void load_texture(void);
	void set_pos(const sf::Vector2f& pos);
	void set_frame_sprite(const sf::Vector2i& frame_pos);
	void move_x(const Key& key);
	void update_x(sf::FloatRect& rect, float dt);
	void move_y(const Key& key);
	void update_y(sf::FloatRect& rect, float dt);
public:
	Turtix(sf::RenderWindow* _window, Map* _map);
	Turtix();
	virtual ~Turtix();
	Turtix(const Turtix& turtix);
	Turtix& operator=(const Turtix& turtix);
	sf::Vector2f get_center_pos(void) const;
	sf::FloatRect get_rect(void) const;
	void update_frame_sprite(void);
	void move(const Key& key, float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif /* __TURTIX_HPP */