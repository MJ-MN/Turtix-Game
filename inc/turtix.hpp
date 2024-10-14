#ifndef __TURTIX_HPP
#define __TURTIX_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "map.hpp"
#include "movement.hpp"

#define TURTIX_SCALE 1.5f

const std::string TURTIX_IMAGE_FILE = "./Sprite/turtix.png";

class Turtix {
private:
	sf::Texture turtixTexture;
	sf::Sprite turtixSprite;
	sf::RenderWindow* window;
	MovementX* movement_x;
	MovementY* movement_y;

	void move_x(sf::FloatRect& turtixRect, const Key* key, float dt);
	void move_y(sf::FloatRect& turtixRect, const Key* key, float dt);
public:
	Turtix(sf::RenderWindow* _window, Map* _map);
	virtual ~Turtix();
	sf::Vector2f get_center_pos(void) const;
	sf::FloatRect get_rect(void) const;
	void draw(void);
	void move(const Key* key, float dt);
};

#endif /* __TURTIX_HPP */