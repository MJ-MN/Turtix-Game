#include <iostream>

#include "../inc/game.hpp"
#include "../inc/map.hpp"
#include "../inc/turtix.hpp"

using namespace std;

TurtixGame::TurtixGame()
{
	this->game_finished = false;
	this->key.up_pressed = false;
	this->key.last_up_pressed = false;
	this->key.right_pressed = false;
	this->key.left_pressed = false;
	this->window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_HEADER_NAME);
	this->window->setFramerateLimit(120);
	this->view = new sf::View(sf::FloatRect(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT));
	this->load_music();
	this->map.setScale(MAP_SCALE, MAP_SCALE);
	this->turtix = new Turtix(this->window, &this->map);
}

TurtixGame::~TurtixGame()
{
	this->music.stop();
	this->window->close();
	delete this->turtix;
	delete this->view;
	delete this->window;
}

void TurtixGame::start(void)
{
	sf::Clock clock;

	while (!game_finished) {
		this->window->clear(SKY_BLUE_COLOR);
		sf::Event event;
		while (this->window->pollEvent(event)) {
			this->handle_event(event);
		}
		if (clock.getElapsedTime().asMilliseconds() >= LOOP_TIME_MS) {
			clock.restart();
			this->turtix->move(key, LOOP_TIME_MS);
			this->turtix->update_frame_sprite();
			this->move_view();
			this->map.update_frames_sprite(this->view->getCenter());
			this->key.last_up_pressed = this->key.up_pressed;
		}
		this->window->setView(*this->view);
		this->window->draw(this->map);
		this->window->draw(*this->turtix);
		this->window->display();
	}
}

void TurtixGame::handle_event(const sf::Event& event)
{
	if (event.type == sf::Event::Closed) {
		this->game_finished = true;
	} else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Up) {
			this->key.up_pressed = true;
		}
		if (event.key.code == sf::Keyboard::Right) {
			this->key.right_pressed = true;
		}
		if (event.key.code == sf::Keyboard::Left) {
			this->key.left_pressed = true;
		}
	} else if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Up) {
			this->key.up_pressed = false;
		}
		if (event.key.code == sf::Keyboard::Right) {
			this->key.right_pressed = false;
		}
		if (event.key.code == sf::Keyboard::Left) {
			this->key.left_pressed = false;
		}
	}
}

void TurtixGame::move_view(void)
{
	sf::Vector2f view_center_pos = this->view->getCenter();
	sf::Vector2f diff_pos = this->turtix->get_center_pos() - view_center_pos;

	if (diff_pos.x > 1.0f * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.x += (diff_pos.x - 1.0f * MAP_SCALED_FRAME_SIZE);
	} else if (diff_pos.x < -1.0f * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.x += (diff_pos.x + 1.0f * MAP_SCALED_FRAME_SIZE);
	}
	if (view_center_pos.x + MAP_WIDTH_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE > (this->map.get_size().x - this->map.get_margin_size().x - 1) * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.x = (this->map.get_size().x - this->map.get_margin_size().x- 1 - MAP_WIDTH_FRAMES / 2.0f) * MAP_SCALED_FRAME_SIZE;
	} else if (view_center_pos.x - MAP_WIDTH_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE < (-this->map.get_margin_size().x + 1) * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.x = (MAP_WIDTH_FRAMES / 2.0f - this->map.get_margin_size().x + 1) * MAP_SCALED_FRAME_SIZE;
	}

	if (diff_pos.y > 1.0f * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.y += (diff_pos.y - 1.0f * MAP_SCALED_FRAME_SIZE);
	} else if (diff_pos.y < -1.0f * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.y += (diff_pos.y + 1.0f * MAP_SCALED_FRAME_SIZE);
	}
	if (view_center_pos.y + MAP_HEIGHT_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE > (this->map.get_size().y - this->map.get_margin_size().y - 1) * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.y = (this->map.get_size().y - this->map.get_margin_size().y - 1 - MAP_HEIGHT_FRAMES / 2.0f) * MAP_SCALED_FRAME_SIZE;
	} else if (view_center_pos.y - MAP_HEIGHT_FRAMES / 2.0f * MAP_SCALED_FRAME_SIZE < (-this->map.get_margin_size().y + 1) * MAP_SCALED_FRAME_SIZE) {
		view_center_pos.y = (MAP_HEIGHT_FRAMES / 2.0f - this->map.get_margin_size().y + 1) * MAP_SCALED_FRAME_SIZE;
	}
	this->view->setCenter(view_center_pos);
}

void TurtixGame::load_music(void)
{
	if (!this->music.openFromFile(MUSIC_FILE)) {
		cerr << "Failed to load " << MUSIC_FILE << "!" << endl;
	}
	this->music.setVolume(20.0f);
	this->music.play();
	this->music.setLoop(true);
}
