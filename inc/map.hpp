#ifndef __MAP_HPP
#define __MAP_HPP

#include <SFML/Graphics.hpp>

#include "manual.hpp"

#define MAP_IMAGE_WIDTH_FRAMES 10
#define MAP_IMAGE_HEIGHT_FRAMES 10
#define MAP_FRAME_SIZE 102
#define MAP_WIDTH_FRAMES 7
#define MAP_HEIGHT_FRAMES 5
#define MAP_SCALE 2.0f
#define MAP_SCALED_FRAME_SIZE (MAP_SCALE * MAP_FRAME_SIZE)
#define MAP_INIT_FRAME 39
#define MAP_HEIGHT_OFFSET_FRAME 1

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

	void load_frames_number(void);
	void load_frames_solidity(void);
	sf::Vector2i find_init_frame(void);
	void load_texture(void);
	sf::Vector2i find_center_frame(const sf::Vector2f center_pos) const;
	void update_frame_sprite(const sf::Vector2i& frame_pos, const sf::Vector2i& perv_frame_pos);
	void update_right_left_frames(int pos_x, int shift_x);
	void update_top_bottom_frames(int pos_y, int shift_y);
	int get_first_corner_frame(int frame_pos, int shift) const;
	int get_frame_number(const sf::Vector2i& frame_pos) const;
	sf::Vector2i check_frame_pos(int x, int y) const;
	sf::FloatRect get_solid_frame_rect(const sf::Vector2i& frame_pos, int solidity);
public:
	Map();
	virtual ~Map();
	sf::Vector2i get_size(void) const;
	sf::Vector2i get_margin_size(void) const;
	void init_frames_sprite(const sf::Vector2i& center_pos);
	void update_frames_sprite(const sf::Vector2f& center_pos);
	float is_valid_x(const sf::FloatRect& turtixRect, float v_x);
	float is_valid_y(const sf::FloatRect& turtixRect, float v_y);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif /* __MAP_HPP */