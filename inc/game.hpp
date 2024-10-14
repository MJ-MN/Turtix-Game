#ifndef __GAME_HPP
#define __GAME_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "map.hpp"
#include "turtix.hpp"
#include "manual.hpp"

#define WINDOW_WIDTH (MAP_SCALE * WIDTH_FRAMES * FRAME_SIZE)
#define WINDOW_HEIGHT (MAP_SCALE * HEIGHT_FRAMES * FRAME_SIZE)
#define SKY_BLUE_COLOR sf::Color(135, 206, 235)

const std::string GAME_HEADER_NAME = "Turtix";

class TurtixGame {
private:
	bool game_finished;
	Key key;
	Map* map; /* TODO */
	sf::RenderWindow* window;
	sf::View* view;
	Turtix* turtix;

	void handle_event(const sf::Event& event);
	void move_view(void);
public:
	TurtixGame();
	virtual ~TurtixGame();
	void start(void);
};

#endif /* __GAME_HPP */