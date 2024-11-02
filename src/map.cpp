#include <iostream>
#include <fstream>
#include <sstream>

#include "../inc/map.hpp"

using namespace std;

Map::Map()
{
	this->bgVertices.setPrimitiveType(sf::Triangles);
	this->bgVertices.resize((MAP_WIDTH_FRAMES + 2) * (MAP_HEIGHT_FRAMES + 2) * POINTS_IN_FRAME);
	this->load_frames_number();
	this->load_frames_solidity();
	this->load_texture();
	this->init_frames_sprite(this->find_init_frame());
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

void Map::load_frames_number(void)
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

void Map::load_texture(void)
{
	if (!this->bgTexture.loadFromFile(BACKGROUND_IMAGE_FILE)) {
		cerr << "Failed to load " << BACKGROUND_IMAGE_FILE << "!" << endl;
	}
}

void Map::init_frames_sprite(const sf::Vector2i& center_pos)
{

	for (int y = -1; y <= MAP_HEIGHT_FRAMES; y++) {
		for (int x = -1; x <= MAP_WIDTH_FRAMES; x++) {
			sf::Vector2i frame_pos(center_pos.x + x - MAP_WIDTH_FRAMES / 2,
				center_pos.y + y - MAP_HEIGHT_FRAMES / 2);
			this->update_frame_sprite(frame_pos, sf::Vector2i(x, y));
		}
	}
	this->top_left_frame = {-1, -1};
	this->last_center_pos = center_pos;
}

void Map::update_frames_sprite(const sf::Vector2f& center_pos)
{
	int shift;
	sf::Vector2i center_frame_pos = this->find_center_frame(center_pos);

	if (center_frame_pos.x != this->last_center_pos.x) {
		shift = center_frame_pos.x - this->last_center_pos.x;
		this->update_right_left_frames(center_frame_pos.x, shift);
		this->last_center_pos.x = center_frame_pos.x;
	}
	if (center_frame_pos.y != this->last_center_pos.y) {
		shift = center_frame_pos.y - this->last_center_pos.y;
		this->update_top_bottom_frames(center_frame_pos.y, shift);
		this->last_center_pos.y = center_frame_pos.y;
	}
}

void Map::update_right_left_frames(int pos_x, int shift_x)
{
	for (int y = -1; y <= MAP_HEIGHT_FRAMES; y++) {
		sf::Vector2i new_frame_pos(
			pos_x + GET_SIGN(shift_x) * MAP_WIDTH_FRAMES / 2 + shift_x,
			this->last_center_pos.y + y - MAP_HEIGHT_FRAMES / 2
		);
		sf::Vector2i curr_frame_pos = check_frame_pos(this->get_first_corner_frame(this->top_left_frame.x, shift_x), this->top_left_frame.y + y + 1);
		this->update_frame_sprite(new_frame_pos, curr_frame_pos);
	}
	this->top_left_frame = check_frame_pos(this->top_left_frame.x + shift_x, this->top_left_frame.y);
}

void Map::update_top_bottom_frames(int pos_y, int shift_y)
{
	for (int x = -1; x <= MAP_WIDTH_FRAMES; x++) {
		sf::Vector2i new_frame_pos(
			this->last_center_pos.x + x - MAP_WIDTH_FRAMES / 2,
			pos_y + GET_SIGN(shift_y) * MAP_HEIGHT_FRAMES / 2 + shift_y
		);
		sf::Vector2i curr_frame_pos = check_frame_pos(this->top_left_frame.x + x + 1, this->get_first_corner_frame(this->top_left_frame.y, shift_y));
		this->update_frame_sprite(new_frame_pos, curr_frame_pos);
	}
	this->top_left_frame = check_frame_pos(this->top_left_frame.x, this->top_left_frame.y + shift_y);
}

int Map::get_first_corner_frame(int frame_pos, int shift) const
{
	return (shift <= 0) ? frame_pos + shift : frame_pos + shift - 1;
}

sf::Vector2i Map::find_center_frame(const sf::Vector2f center_pos) const
{
	sf::Vector2i center_frame_pos = {
		(int)(center_pos.x / MAP_SCALED_FRAME_SIZE),
		(int)(center_pos.y / MAP_SCALED_FRAME_SIZE)
	};
	return center_frame_pos;
}

void Map::update_frame_sprite(const sf::Vector2i& frame_pos, const sf::Vector2i& perv_frame_pos)
{
	int frame_number = get_frame_number(frame_pos);
	sf::Vertex* squares = &(this->bgVertices[((perv_frame_pos.x + 1) + (perv_frame_pos.y + 1) * (MAP_WIDTH_FRAMES + 2)) * POINTS_IN_FRAME]);

	squares[0].position = sf::Vector2f(frame_pos.x * MAP_FRAME_SIZE, frame_pos.y * MAP_FRAME_SIZE);
	squares[1].position = squares[4].position = sf::Vector2f(frame_pos.x * MAP_FRAME_SIZE, (frame_pos.y + 1) * MAP_FRAME_SIZE);
	squares[2].position = squares[3].position = sf::Vector2f((frame_pos.x + 1) * MAP_FRAME_SIZE, frame_pos.y * MAP_FRAME_SIZE);
	squares[5].position = sf::Vector2f((frame_pos.x + 1) * MAP_FRAME_SIZE, (frame_pos.y + 1) * MAP_FRAME_SIZE);
	if (frame_number != -1) {
		sf::Vector2i tile_frame_pos(frame_number % MAP_IMAGE_WIDTH_FRAMES, frame_number / MAP_IMAGE_WIDTH_FRAMES);
		squares[0].texCoords = sf::Vector2f(tile_frame_pos.x * MAP_FRAME_SIZE, tile_frame_pos.y * MAP_FRAME_SIZE);
		squares[1].texCoords = squares[4].texCoords = sf::Vector2f(tile_frame_pos.x * MAP_FRAME_SIZE, (tile_frame_pos.y + 1) * MAP_FRAME_SIZE);
		squares[2].texCoords = squares[3].texCoords = sf::Vector2f((tile_frame_pos.x + 1) * MAP_FRAME_SIZE, tile_frame_pos.y * MAP_FRAME_SIZE);
		squares[5].texCoords = sf::Vector2f((tile_frame_pos.x + 1) * MAP_FRAME_SIZE, (tile_frame_pos.y + 1) * MAP_FRAME_SIZE);
	} else {
		squares[0].texCoords = sf::Vector2f((MAP_IMAGE_WIDTH_FRAMES - 1) * MAP_FRAME_SIZE, (MAP_IMAGE_HEIGHT_FRAMES - 1) * MAP_FRAME_SIZE);
		squares[1].texCoords = squares[4].texCoords = sf::Vector2f((MAP_IMAGE_WIDTH_FRAMES - 1) * MAP_FRAME_SIZE, MAP_IMAGE_HEIGHT_FRAMES * MAP_FRAME_SIZE);
		squares[2].texCoords = squares[3].texCoords = sf::Vector2f(MAP_IMAGE_WIDTH_FRAMES * MAP_FRAME_SIZE, (MAP_IMAGE_HEIGHT_FRAMES - 1) * MAP_FRAME_SIZE);
		squares[5].texCoords = sf::Vector2f(MAP_IMAGE_WIDTH_FRAMES * MAP_FRAME_SIZE, MAP_IMAGE_HEIGHT_FRAMES * MAP_FRAME_SIZE);
	}
}

int Map::get_frame_number(const sf::Vector2i& frame_pos) const
{
	return this->bg_frame_numbers[frame_pos.y + map_margin.y][frame_pos.x + map_margin.x];
}

sf::Vector2i Map::check_frame_pos(int x, int y) const
{
	if (x > MAP_WIDTH_FRAMES) {
		x -= (MAP_WIDTH_FRAMES + 2);
	}
	if (x < -1) {
		x += (MAP_WIDTH_FRAMES + 2);
	}
	if (y > MAP_HEIGHT_FRAMES) {
		y -= (MAP_HEIGHT_FRAMES + 2);
	}
	if (y < -1) {
		y += (MAP_HEIGHT_FRAMES + 2);
	}
	return sf::Vector2i(x, y);
}

sf::Vector2i Map::find_init_frame(void)
{
	sf::Vector2i init_frame_pos(MAP_WIDTH_FRAMES / 2, MAP_HEIGHT_FRAMES / 2);

	for (int y = 0; y < this->bg_frame_numbers.size(); y++) {
		for (int x = 0; x < this->bg_frame_numbers[y].size(); x++) {
			if (this->bg_frame_numbers[y][x] == MAP_INIT_FRAME) {
				this->map_margin = {
					x - MAP_WIDTH_FRAMES / 2,
					y - MAP_HEIGHT_FRAMES / 2 - MAP_HEIGHT_OFFSET_FRAME
				};
				init_frame_pos = { x, y };
				return init_frame_pos - this->map_margin;
			}
		}
	}
	return init_frame_pos;
}

sf::FloatRect Map::get_solid_rect(const sf::Vector2i& frame_pos, int solidity)
{
	int frame_number = get_frame_number(frame_pos);
	sf::Vector2f rect_pos(0.0f, 0.0f);
	sf::Vector2f rect_size(0.0f, 0.0f);

	if (frame_number == -1) {
		return sf::FloatRect(rect_pos, rect_size);
	}
	rect_size = {
		MAP_SCALED_FRAME_SIZE * 0.6f,
		MAP_SCALED_FRAME_SIZE * 0.6f
	};
	switch (this->frames_solidity[frame_number] & solidity) {
	case Solidity::TOP_LEFT:
		rect_pos = { (float)frame_pos.x * MAP_SCALED_FRAME_SIZE, (float)frame_pos.y * MAP_SCALED_FRAME_SIZE };
		break;
	case Solidity::DOWN_LEFT:
		rect_pos = { (float)frame_pos.x * MAP_SCALED_FRAME_SIZE, (frame_pos.y + 0.4f) * MAP_SCALED_FRAME_SIZE };
		break;
	case Solidity::TOP_RIGHT:
		rect_pos = { (frame_pos.x + 0.4f) * MAP_SCALED_FRAME_SIZE, (float)frame_pos.y * MAP_SCALED_FRAME_SIZE };
		break;
	case Solidity::DOWN_RIGHT:
		rect_pos = { (frame_pos.x + 0.4f) * MAP_SCALED_FRAME_SIZE, (frame_pos.y + 0.4f) * MAP_SCALED_FRAME_SIZE };
		break;
	}
	return sf::FloatRect(rect_pos, rect_size);
}

sf::FloatRect Map::get_ladder_rect(const sf::Vector2i& frame_pos)
{
	int frame_number = get_frame_number(frame_pos);
	sf::Vector2f rect_pos(0.0f, 0.0f);
	sf::Vector2f rect_size(0.0f, 0.0f);

	switch (frame_number) {
	case 25:
		rect_pos = { (frame_pos.x + 0.45f) * MAP_SCALED_FRAME_SIZE, (frame_pos.y + 0.4f) * MAP_SCALED_FRAME_SIZE };
		rect_size = { 0.1f * MAP_SCALED_FRAME_SIZE, 0.6f * MAP_SCALED_FRAME_SIZE };
		break;
	case 26:
	case 27:
	case 28:
		rect_pos = { (frame_pos.x + 0.45f) * MAP_SCALED_FRAME_SIZE, (float)frame_pos.y * MAP_SCALED_FRAME_SIZE };
		rect_size = { 0.1f * MAP_SCALED_FRAME_SIZE, MAP_SCALED_FRAME_SIZE };
		break;
	case 29:
		rect_pos = { (frame_pos.x + 0.45f) * MAP_SCALED_FRAME_SIZE, (float)frame_pos.y * MAP_SCALED_FRAME_SIZE };
		rect_size = { 0.1f * MAP_SCALED_FRAME_SIZE, 0.6f * MAP_SCALED_FRAME_SIZE };
		break;
	}
	return sf::FloatRect(rect_pos, rect_size);
}

void Map::find_nearby_frames(const sf::FloatRect& rect, sf::Vector2i* frame_pos)
{
	frame_pos[0] = { (int)(rect.left / MAP_SCALED_FRAME_SIZE), (int)(rect.top / MAP_SCALED_FRAME_SIZE) };
	frame_pos[1] = {(int)(rect.left / MAP_SCALED_FRAME_SIZE) , (int)((rect.top + rect.height) / MAP_SCALED_FRAME_SIZE) };
	frame_pos[2] = {(int)((rect.left + rect.width) / MAP_SCALED_FRAME_SIZE), (int)(rect.top / MAP_SCALED_FRAME_SIZE) };
	frame_pos[3] = {(int)((rect.left + rect.width) / MAP_SCALED_FRAME_SIZE), (int)((rect.top + rect.height) / MAP_SCALED_FRAME_SIZE) };
}

float Map::is_valid_x(const sf::FloatRect& rect, float v_x)
{
	sf::FloatRect solidRect;
	sf::Vector2i frame_pos[4];
	float penalty_x = 0.0f;

	find_nearby_frames(rect, frame_pos);
	for (int i = 0; i < 4; i++) {
		if (v_x > 0.0f) {
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::TOP_RIGHT);
			if (solidRect.width != 0.0f && rect.intersects(solidRect)) {
				penalty_x = solidRect.left - rect.left - rect.width - 1;
			}
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::DOWN_RIGHT);
			if (solidRect.width != 0.0f && rect.intersects(solidRect)) {
				penalty_x = solidRect.left - rect.left - rect.width - 1;
			}
		}
		if (v_x < 0.0f) {
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::TOP_LEFT);
			if (solidRect.width != 0.0f && rect.intersects(solidRect)) {
				penalty_x = solidRect.left + solidRect.width - rect.left + 1;
			}
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::DOWN_LEFT);
			if (solidRect.width != 0.0f && rect.intersects(solidRect)) {
				penalty_x = solidRect.left + solidRect.width - rect.left + 1;
			}
		}
	}
	return penalty_x;
}

float Map::is_valid_y(const sf::FloatRect& rect, float v_y)
{
	sf::FloatRect solidRect;
	sf::Vector2i frame_pos[4];
	float penalty_y = 0.0f;

	find_nearby_frames(rect, frame_pos);
	for (int i = 0; i < 4; i++) {
		if (v_y > 0.0f) {
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::DOWN_LEFT);
			if (solidRect.height != 0.0f && rect.intersects(solidRect)) {
				penalty_y = solidRect.top - rect.top - rect.height - 1;
			}
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::DOWN_RIGHT);
			if (solidRect.height != 0.0f && rect.intersects(solidRect)) {
				penalty_y = solidRect.top - rect.top - rect.height - 1;
			}
		}
		if (v_y < 0.0f) {
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::TOP_LEFT);
			if (solidRect.height != 0.0f && rect.intersects(solidRect)) {
				penalty_y = solidRect.top + solidRect.height - rect.top + 1;
			}
			solidRect = this->get_solid_rect(frame_pos[i], Solidity::TOP_RIGHT);
			if (solidRect.height != 0.0f && rect.intersects(solidRect)) {
				penalty_y = solidRect.top + solidRect.height - rect.top + 1;
			}
		}
	}
	return penalty_y;
}

bool Map::is_on_the_ladder(sf::FloatRect& rect)
{
	sf::FloatRect ladderRect;
	sf::Vector2i frame_pos[4];

	find_nearby_frames(rect, frame_pos);
	for (int i = 0; i < 4; i++) {
		ladderRect = this->get_ladder_rect(frame_pos[i]);
		if (rect.width != 0.0f && rect.intersects(ladderRect)) {
			rect.left = (frame_pos[i].x + 0.5f) * MAP_SCALED_FRAME_SIZE - 0.5f * rect.width;
			return true;
		}
	}
	return false;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &this->bgTexture;
	target.draw(this->bgVertices, states);
}
