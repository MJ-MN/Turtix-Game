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

const std::string TURTIX_LEFT_IMAGE_FILE = "./Sprite/turtix_left.png";
const std::string TURTIX_RIGHT_IMAGE_FILE = "./Sprite/turtix_right.png";
const std::string TURTIX_UP_IMAGE_FILE = "./Sprite/turtix_up.png";

class Turtix : public sf::Drawable {
private:
	Direction direction;
	sf::VertexArray vertices;
	sf::Texture textureLeft;
	sf::Texture textureRight;
	sf::Texture textureUp;
	sf::RenderWindow* window;
	MovementX* movement_x;
	MovementY* movement_y;

	void load_texture(void);
	void init_frame_sprite(void);
	void move_x(sf::FloatRect& turtixRect, const Key& key, float dt);
	void move_y(sf::FloatRect& turtixRect, const Key& key, float dt);
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