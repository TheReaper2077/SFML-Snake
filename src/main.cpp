#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <vector>

int gridsize = 20, speed = 20, HEIGHT = 600, WIDTH = 800, player_length = 4, velX = 0, velY = 0, score, pposX, pposY, aposX, aposY;

sf::RectangleShape renderRect(sf::Vector2f(gridsize, gridsize));
std::vector<sf::Vector2f> player;

void keyPressed(int key) {
	if (key == 22 && velY <= 0) {
		velX = 0;
		velY = -speed;
	}
	if (key == 0 && velX <= 0) {
		velX = -speed;
		velY = 0;
	}
	if (key == 18 && velY >= 0) {
		velX = 0;
		velY = speed;
	}
	if (key == 3 && velX >= 0) {
		velX = speed;
		velY = 0;
	}
}

int game(sf::RenderWindow &window) {
	bool apple_eaten = false;;
	if (aposX == pposX && aposY == pposY) {
		score += 10;
		std::cout << score << std::endl;
		aposX = (rand() % (WIDTH / gridsize))*gridsize;
		aposY = (rand() % (HEIGHT / gridsize))*gridsize;
		apple_eaten = true;
	}

	pposX += velX;
	pposY += velY;

	if (pposX > WIDTH - gridsize) {
		pposX = 0;
	}
	if (pposX < 0) {
		pposX = WIDTH - gridsize;
	}
	if (pposY > HEIGHT - gridsize) {
		pposY = 0;
	}
	if (pposY < 0) {
		pposY = HEIGHT - gridsize;
	}

	sf::Vector2f first;

	if (pposX % gridsize == 0 && pposY % gridsize == 0) {
		for (auto i = player.begin(); i != player.end(); i++) {
			if (i == player.begin()) {
				first = *i;
				i->x = pposX;
				i->y = pposY;
			} else {
				if (*i == sf::Vector2f(pposX, pposY)) {
					return 1;
				}
				std::swap(*i, first);
			}
		}
		if (apple_eaten) {
			player.push_back(first);
		}
		
		renderRect.setOutlineThickness(2);
		renderRect.setOutlineColor(sf::Color::Black);

		for (auto i = player.begin(); i != player.end(); i++) {
			renderRect.setPosition(*i);
			renderRect.setFillColor(sf::Color::Red);
			window.draw(renderRect);
		}

		renderRect.setFillColor(sf::Color::Cyan);
		renderRect.setPosition(sf::Vector2f(aposX, aposY));
		window.draw(renderRect);
	}

	return 0;
}

void reset() {
	player.clear();
	score = 0;
	pposX = aposX = (rand() % (WIDTH / gridsize))*gridsize;
	pposY = aposY = (rand() % (HEIGHT / gridsize))*gridsize;
	int keys[4] = {22, 0, 18, 3};
	keyPressed(keys[rand() % 4]);

	for (int i = 0; i != player_length; i++) {
		player.push_back(sf::Vector2f(pposX, pposY));
	}
}

int main(int, char**) {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake");

	reset();

	sf::Clock clock;
	
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) keyPressed(event.key.code);
		}

		if (clock.getElapsedTime().asMilliseconds() >= 50) {
			window.clear(sf::Color::Black);
			if (game(window)) {
				std::cout << "Game Over! Score: " << score << std::endl;
				reset();
			};
			window.display();
			clock.restart();
		}   
	}

	return 0;
}
