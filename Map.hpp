#pragma once

#include <SFML/Graphics.hpp>
#include "globals.hpp"

struct Tile {
	float f, g, h;
	bool traversable;
	bool open;
	bool closed;
	sf::Vector2i parent;
};

class TileMap {
	public:
		Tile map[NUM_OF_TILES + 2][NUM_OF_TILES + 2];
		Tile& tile(sf::Vector2i index);
		float tileSize;
		sf::Vector2i start;
		sf::Vector2i goal;
		TileMap();
		void update();
		void draw(sf::RenderTarget& w);
	private:

		void restartAstar();
		bool pathFound;
		std::vector<sf::Vector2i> openList;
};
