#ifndef __MAP_HPP
#define __MAP_HPP

#include <SFML/Graphics.hpp>

#include "manual.hpp"

#define IMAGE_WIDTH_FRAME 10
#define IMAGE_HEIGHT_FRAME 10
#define FRAME_SIZE 102
#define WIDTH_FRAMES 7
#define HEIGHT_FRAMES 5
#define POINTS_IN_FRAME 6
#define MAP_SCALE 2
#define SCALED_FRAME_SIZE (MAP_SCALE * FRAME_SIZE)
#define INIT_FRAME 39
#define HEIGHT_OFFSET_FRAME 1

enum Solidity {
	TOP_LEFT = 0x01,
	DOWN_LEFT = 0x02,
	TOP_RIGHT = 0x04,
	DOWN_RIGHT = 0x08
};

const std::string BACKGROUND_TEXT_FILE = "./Level1.txt";
const std::string BACKGROUND_IMAGE_FILE = "./Sprite/tile.png";
const std::string SOLIDITY_TEXT_FILE = "./Sprite/tile solidity.txt";

class Map : public sf::Drawable, public sf::Transformable {
private:
	std::vector<std::vector<int>> bg_frame_numbers;
	std::map<int, int> frames_solidity;
	sf::Vector2i last_center_pos;
	sf::Vector2i top_left_frame;
	sf::Vector2i map_margin;
	sf::VertexArray bgVertices;
	sf::Texture bgTexture;
	sf::View* view;
	sf::RenderWindow* window;

	void load_background_frame_numbers(void);
	void load_frames_solidity(void);
	sf::Vector2i find_init_frame(void) const;
	void load_background_texture(void);
	void update_frame_sprite(const sf::Vector2i& frame_pos, const sf::Vector2i& perv_frame_pos);
	int get_frame_number(const sf::Vector2i& frame_pos) const;
	sf::Vector2i check_frame_pos(int x, int y) const;
	sf::FloatRect get_solid_frame_rect(const sf::Vector2i& frame_pos, int solidity);
public:
	Map(sf::RenderWindow* _window); /* TODO */
	virtual ~Map();
	int get_cols(void) const;
	int get_rows(void) const;
	int get_margin_col(void) const;
	int get_margin_row(void) const;
	void init_background_sprite(const sf::Vector2i& center_pos);
	void update_background_sprite(const sf::Vector2i& center_pos);
	sf::Vector2i find_center_frame(const sf::View* view) const;
	float is_valid_x(const sf::FloatRect& turtixRect, float v_x);
	float is_valid_y(const sf::FloatRect& turtixRect, float v_y);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif /* __MAP_HPP */