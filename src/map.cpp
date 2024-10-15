#include <iostream>
#include <fstream>
#include <sstream>

#include "../inc/map.hpp"

using namespace std;

Map::Map(sf::RenderWindow* _window)
{
	window = _window;
	this->bgVertices.setPrimitiveType(sf::Triangles);
	this->bgVertices.resize((WIDTH_FRAMES + 2) * (HEIGHT_FRAMES + 2) * POINTS_IN_FRAME);
	this->load_background_frame_numbers();
	this->load_frames_solidity();
	this->load_background_texture();
	this->init_background_sprite(this->find_init_frame());
}

Map::~Map()
{

}

sf::Vector2i Map::get_size(void) const
{
	return sf::Vector2i(
		this->bg_frame_numbers[0].size(),
		this->bg_frame_numbers.size()
	);
}

sf::Vector2i Map::get_margin_size(void) const
{
	return this->map_margin;
}

void Map::load_background_frame_numbers(void)
{
	string line;
	int frame_number;
	vector<int> frame_numbers;
	stringstream ss;
	ifstream bgFile(BACKGROUND_TEXT_FILE);

	while (getline(bgFile, line)) {
		ss << line;
		while (ss >> frame_number) {
			frame_numbers.push_back(frame_number);
		}
		ss.clear();
		this->bg_frame_numbers.push_back(frame_numbers);
		frame_numbers.clear();
	}
	bgFile.close();
}

void Map::load_frames_solidity(void)
{
	int frame_number = 0, solidity = 0;
	ifstream solidityFile(SOLIDITY_TEXT_FILE);

	while (solidityFile >> solidity) {
		frames_solidity[frame_number++] = solidity;
	}
	solidityFile.close();
}

void Map::load_background_texture(void)
{
	if (!this->bgTexture.loadFromFile(BACKGROUND_IMAGE_FILE)) {
		cerr << "Failed to load " << BACKGROUND_IMAGE_FILE << "!" << endl;
	}
}

void Map::init_background_sprite(const sf::Vector2i& center_pos)
{

	for (int y = -1; y <= HEIGHT_FRAMES; y++) {
		for (int x = -1; x <= WIDTH_FRAMES; x++) {
			sf::Vector2i frame_pos(center_pos.x + x - WIDTH_FRAMES / 2,
				center_pos.y + y - HEIGHT_FRAMES / 2);
			this->update_frame_sprite(frame_pos, sf::Vector2i(x, y));
		}
	}
	this->top_left_frame = {-1, -1};
	this->last_center_pos = center_pos;
}

void Map::update_background_sprite(const sf::Vector2i& center_pos)
{
	if (center_pos.x > last_center_pos.x) {
		this->update_right_frames(center_pos.x);
	} else if (center_pos.x < last_center_pos.x) {
		this->update_left_frames(center_pos.x);
	}
	this->last_center_pos.x = center_pos.x;
	if (center_pos.y > last_center_pos.y) {
		this->update_bottom_frames(center_pos.y);
	} else if (center_pos.y < last_center_pos.y) {
		this->update_top_frames(center_pos.y);
	}
	this->last_center_pos.y = center_pos.y;
}

void Map::update_right_frames(int pos_x)
{
	for (int y = -1; y <= HEIGHT_FRAMES; y++) {
		sf::Vector2i new_frame_pos(
			pos_x + WIDTH_FRAMES / 2 + 1,
			this->last_center_pos.y + y - HEIGHT_FRAMES / 2
		);
		sf::Vector2i curr_frame_pos = check_frame_pos(this->top_left_frame.x, this->top_left_frame.y + y + 1);
		this->update_frame_sprite(new_frame_pos, curr_frame_pos);
	}
	this->top_left_frame = check_frame_pos(this->top_left_frame.x + 1, this->top_left_frame.y);
}

void Map::update_left_frames(int pos_x)
{
	for (int y = -1; y <= HEIGHT_FRAMES; y++) {
		sf::Vector2i new_frame_pos(
			pos_x - WIDTH_FRAMES / 2 - 1,
			this->last_center_pos.y + y - HEIGHT_FRAMES / 2
		);
		sf::Vector2i curr_frame_pos = check_frame_pos(this->top_left_frame.x + WIDTH_FRAMES + 1, this->top_left_frame.y + y + 1);
		this->update_frame_sprite(new_frame_pos, curr_frame_pos);
	}
	this->top_left_frame = check_frame_pos(this->top_left_frame.x - 1, this->top_left_frame.y);
}

void Map::update_bottom_frames(int pos_y)
{
	for (int x = -1; x <= WIDTH_FRAMES; x++) {
		sf::Vector2i new_frame_pos(
			this->last_center_pos.x + x - WIDTH_FRAMES / 2,
			pos_y + HEIGHT_FRAMES / 2 + 1
		);
		sf::Vector2i curr_frame_pos = check_frame_pos(this->top_left_frame.x + x + 1, this->top_left_frame.y);
		this->update_frame_sprite(new_frame_pos, curr_frame_pos);
	}
	this->top_left_frame = check_frame_pos(this->top_left_frame.x, this->top_left_frame.y + 1);
}

void Map::update_top_frames(int pos_y)
{
	for (int x = -1; x <= WIDTH_FRAMES; x++) {
		sf::Vector2i new_frame_pos(
			this->last_center_pos.x + x - WIDTH_FRAMES / 2,
			pos_y - HEIGHT_FRAMES / 2 - 1
		);
		sf::Vector2i curr_frame_pos = check_frame_pos(this->top_left_frame.x + x + 1, this->top_left_frame.y + HEIGHT_FRAMES + 1);
		this->update_frame_sprite(new_frame_pos, curr_frame_pos);
	}
	this->top_left_frame = check_frame_pos(this->top_left_frame.x, this->top_left_frame.y - 1);
}

void Map::update_frame_sprite(const sf::Vector2i& frame_pos, const sf::Vector2i& perv_frame_pos)
{
	int frame_number = get_frame_number(frame_pos);
	sf::Vertex* squares = &(this->bgVertices[((perv_frame_pos.x + 1) + (perv_frame_pos.y + 1) * (WIDTH_FRAMES + 2)) * POINTS_IN_FRAME]);

	squares[0].position = sf::Vector2f(frame_pos.x * FRAME_SIZE, frame_pos.y * FRAME_SIZE);
	squares[1].position = squares[4].position = sf::Vector2f(frame_pos.x * FRAME_SIZE, (frame_pos.y + 1) * FRAME_SIZE);
	squares[2].position = squares[3].position = sf::Vector2f((frame_pos.x + 1) * FRAME_SIZE, frame_pos.y * FRAME_SIZE);
	squares[5].position = sf::Vector2f((frame_pos.x + 1) * FRAME_SIZE, (frame_pos.y + 1) * FRAME_SIZE);
	if (frame_number != -1) {
		sf::Vector2i tile_frame_pos(frame_number % 10, frame_number / 10);
		squares[0].texCoords = sf::Vector2f(tile_frame_pos.x * FRAME_SIZE, tile_frame_pos.y * FRAME_SIZE);
		squares[1].texCoords = squares[4].texCoords = sf::Vector2f(tile_frame_pos.x * FRAME_SIZE, (tile_frame_pos.y + 1) * FRAME_SIZE);
		squares[2].texCoords = squares[3].texCoords = sf::Vector2f((tile_frame_pos.x + 1) * FRAME_SIZE, tile_frame_pos.y * FRAME_SIZE);
		squares[5].texCoords = sf::Vector2f((tile_frame_pos.x + 1) * FRAME_SIZE, (tile_frame_pos.y + 1) * FRAME_SIZE);
	} else {
		squares[0].texCoords = sf::Vector2f((IMAGE_WIDTH_FRAME - 1) * FRAME_SIZE, (IMAGE_HEIGHT_FRAME - 1) * FRAME_SIZE);
		squares[1].texCoords = squares[4].texCoords = sf::Vector2f((IMAGE_WIDTH_FRAME - 1) * FRAME_SIZE, IMAGE_HEIGHT_FRAME * FRAME_SIZE);
		squares[2].texCoords = squares[3].texCoords = sf::Vector2f(IMAGE_WIDTH_FRAME * FRAME_SIZE, (IMAGE_HEIGHT_FRAME - 1) * FRAME_SIZE);
		squares[5].texCoords = sf::Vector2f(IMAGE_WIDTH_FRAME * FRAME_SIZE, IMAGE_HEIGHT_FRAME * FRAME_SIZE);
	}
}

int Map::get_frame_number(const sf::Vector2i& frame_pos) const
{
	return this->bg_frame_numbers[frame_pos.y + map_margin.y][frame_pos.x + map_margin.x];
}

sf::Vector2i Map::check_frame_pos(int x, int y) const
{
	if (x > WIDTH_FRAMES) {
		x -= (WIDTH_FRAMES + 2);
	}
	if (x < -1) {
		x += (WIDTH_FRAMES + 2);
	}
	if (y > HEIGHT_FRAMES) {
		y -= (HEIGHT_FRAMES + 2);
	}
	if (y < -1) {
		y += (HEIGHT_FRAMES + 2);
	}
	return sf::Vector2i(x, y);
}

sf::Vector2i Map::find_init_frame(void)
{
	sf::Vector2i init_frame_pos(WIDTH_FRAMES / 2, HEIGHT_FRAMES / 2);

	for (int y = 0; y < this->bg_frame_numbers.size(); y++) {
		for (int x = 0; x < this->bg_frame_numbers[y].size(); x++) {
			if (this->bg_frame_numbers[y][x] == INIT_FRAME) {
				this->map_margin = {
					x - WIDTH_FRAMES / 2,
					y - HEIGHT_FRAMES / 2 - HEIGHT_OFFSET_FRAME
				};
				init_frame_pos = { x, y };
				return init_frame_pos - map_margin;
			}
		}
	}
	return init_frame_pos;
}

sf::Vector2i Map::find_center_frame(const sf::View* view) const
{
	sf::Vector2f view_center_pos = view->getCenter();
	sf::Vector2i center_frame_pos = {
		(int)(view_center_pos.x / SCALED_FRAME_SIZE),
		(int)(view_center_pos.y / SCALED_FRAME_SIZE)
	};
	return center_frame_pos;
}

sf::FloatRect Map::get_solid_frame_rect(const sf::Vector2i& frame_pos, int solidity)
{
	int frame_number = get_frame_number(frame_pos);
	sf::Vector2f rect_pos(0.0f, 0.0f);
	sf::Vector2f rect_size(SCALED_FRAME_SIZE * 0.6f, SCALED_FRAME_SIZE * 0.6f);

	if (frame_number == -1) {
		return sf::FloatRect(rect_pos, rect_size);
	}
	switch (this->frames_solidity[frame_number] & solidity) {
	case Solidity::TOP_LEFT:
		rect_pos = { (float)frame_pos.x * SCALED_FRAME_SIZE, (float)frame_pos.y * SCALED_FRAME_SIZE };
		break;
	case Solidity::DOWN_LEFT:
		rect_pos = { (float)frame_pos.x * SCALED_FRAME_SIZE, (frame_pos.y + 0.4f) * SCALED_FRAME_SIZE };
		break;
	case Solidity::TOP_RIGHT:
		rect_pos = { (frame_pos.x + 0.4f) * SCALED_FRAME_SIZE, (float)frame_pos.y * SCALED_FRAME_SIZE };
		break;
	case Solidity::DOWN_RIGHT:
		rect_pos = { (frame_pos.x + 0.4f) * SCALED_FRAME_SIZE, (frame_pos.y + 0.4f) * SCALED_FRAME_SIZE };
		break;
	}
	return sf::FloatRect(rect_pos, rect_size);
}

float Map::is_valid_x(const sf::FloatRect& turtixRect, float v_x)
{
	sf::FloatRect solidFrameRect;
	float penalty_x = 0.0f;
	sf::Vector2i frame_pos[4] = {
		{(int)turtixRect.left / SCALED_FRAME_SIZE, (int)turtixRect.top / SCALED_FRAME_SIZE},
		{(int)turtixRect.left / SCALED_FRAME_SIZE , (int)(turtixRect.top + turtixRect.height) / SCALED_FRAME_SIZE},
		{(int)(turtixRect.left + turtixRect.width) / SCALED_FRAME_SIZE, (int)turtixRect.top / SCALED_FRAME_SIZE},
		{(int)(turtixRect.left + turtixRect.width) / SCALED_FRAME_SIZE, (int)(turtixRect.top + turtixRect.height) / SCALED_FRAME_SIZE}
	};

	for (int i = 0; i < 4; i++) {
		if (v_x > 0.0f) {
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::TOP_RIGHT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_x = solidFrameRect.left - turtixRect.left - turtixRect.width;
			}
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::DOWN_RIGHT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_x = solidFrameRect.left - turtixRect.left - turtixRect.width;
			}
		}
		if (v_x < 0.0f) {
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::TOP_LEFT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_x = solidFrameRect.left + solidFrameRect.width - turtixRect.left;
			}
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::DOWN_LEFT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_x = solidFrameRect.left + solidFrameRect.width - turtixRect.left;
			}
		}
	}
	return penalty_x;
}

float Map::is_valid_y(const sf::FloatRect& turtixRect, float v_y)
{
	sf::FloatRect solidFrameRect;
	float penalty_y = 0.0f;
	sf::Vector2i frame_pos[4] = {
		{(int)turtixRect.left / SCALED_FRAME_SIZE, (int)turtixRect.top / SCALED_FRAME_SIZE},
		{(int)turtixRect.left / SCALED_FRAME_SIZE, (int)(turtixRect.top + turtixRect.height) / SCALED_FRAME_SIZE},
		{(int)(turtixRect.left + turtixRect.width) / SCALED_FRAME_SIZE, (int)turtixRect.top / SCALED_FRAME_SIZE},
		{(int)(turtixRect.left + turtixRect.width) / SCALED_FRAME_SIZE, (int)(turtixRect.top + turtixRect.height) / SCALED_FRAME_SIZE}
	};

	for (int i = 0; i < 4; i++) {
		if (v_y > 0.0f) {
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::DOWN_LEFT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_y = solidFrameRect.top - turtixRect.top - turtixRect.height;
			}
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::DOWN_RIGHT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_y = solidFrameRect.top - turtixRect.top - turtixRect.height;
			}
		}
		if (v_y < 0.0f) {
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::TOP_LEFT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_y = solidFrameRect.top + solidFrameRect.height - turtixRect.top;
			}
			solidFrameRect = this->get_solid_frame_rect(frame_pos[i], Solidity::TOP_RIGHT);
			if (solidFrameRect.height != 0.0f && turtixRect.intersects(solidFrameRect)) {
				penalty_y = solidFrameRect.top + solidFrameRect.height - turtixRect.top;
			}
		}
	}
	return penalty_y;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &this->bgTexture;
	target.draw(this->bgVertices, states);
}
