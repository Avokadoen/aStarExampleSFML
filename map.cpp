#include "Map.hpp"
#include "globals.hpp"
#include <iostream>

TileMap::TileMap(){
	// 	we set fixed start and goal tiles
	start = sf::Vector2i(1, 1);
	goal = sf::Vector2i(NUM_OF_TILES, NUM_OF_TILES);

	tileSize = WINDOW_Y / ((float) NUM_OF_TILES+1);

	/*
		here you should initialise the traversable-variable of all tiles.
		Tiles with indices (i,numTiles), (numTiles, i), (0,i), (i,0)
		constitute the boundary of the tilemap,
		and should not be traversable.
		All other tiles should be traversable
	*/

	for(int i = 0; i < (NUM_OF_TILES + 2); i++){
		for(int j = 0; j < (NUM_OF_TILES + 2); j++){
			map[i][j].traversable = false;

			if(i > 0 && j > 0 && i <= NUM_OF_TILES && j <= NUM_OF_TILES){
				map[i][j].traversable = true;
				map[i][j].h = sqrt((pow(i - goal.x, 2) + pow(j - goal.y, 2)));
			}
		}
	}

	// 	initialise other aspects of the tiles
	//tileSize = windowSize / ((float) numTiles+1);
	restartAstar();
}

void TileMap::update(){
	// we reset states to prepare for another run
	restartAstar();
	// we set the state on the starting tile and add it to the open list
	tile(start).g = 0;
	tile(start).h = sqrt(pow(start.x - goal.x, 2) + pow(start.y - goal.y, 2));// insert your heuristic distance from start to goal
	tile(start).f = tile(start).g + tile(start).h;
	tile(start).open = true;
	openList.push_back(start);
	while (!openList.empty()) {
		// remove smallest from open list (we know it is not empty)
		auto smallest = std::min_element(openList.begin(), openList.end(),
			[&](const sf::Vector2i& v, const sf::Vector2i& w) {
				return tile(v).f < tile(w).f;
			});

		// set current to smallest, close it and remove from openList
		sf::Vector2i current = *smallest;
		openList.erase(smallest);
		tile(current).closed = true;

		if (current == goal) {
			pathFound = true;
			break;
		}

		const sf::Vector2i nbIndex[4] = {
			sf::Vector2i(-1,0), sf::Vector2i(1,0),
			sf::Vector2i(0,1), sf::Vector2i(0,-1)
		};

		for (int i = 0; i < 4; i++) {
			sf::Vector2i nb = current + nbIndex[i];
			// if tile(nb) is non-traversable or closed - ignore
			if(!tile(nb).traversable || tile(nb).closed){

			}
			else{
				if(!tile(nb).open){
					tile(nb).open = true;
					openList.push_back(nb);
				}
				if(tile(nb).open){
					//sf::sleep(sf::milliseconds(10));
					std::cout << "\n" << tile(nb).g << " " << (tile(current).g + COST);
					if(tile(nb).g > (tile(current).g + COST)){
						tile(nb).g = tile(current).g + COST;
						tile(nb).f = tile(nb).g + tile(nb).h;
						tile(nb).parent = current;
						std::cout << "\nf: " << tile(nb).h << "\n";
						std::cout << "\nf: " << tile(nb).f << "\n";
						std::cout << map[nb.x][nb.y].parent.x << " " << map[nb.x][nb.y].parent.y;
					}
				}
			}
			// start får aldri gitt verdier



		}
	}
}

Tile& TileMap::tile(sf::Vector2i index) {
	return map[index.x][index.y];
}

void TileMap::draw(sf::RenderTarget& w){
	sf::CircleShape shape;
	shape.setOrigin(tileSize / 2, tileSize / 2);
	shape.setRadius(tileSize / 2);
	for (int i = 0; i < NUM_OF_TILES + 2; i++) {
		for (int j = 0; j < NUM_OF_TILES + 2; j++) {
			shape.setPosition(i*tileSize, j*tileSize);
			sf::Color color;

			if (!map[i][j].traversable) color = sf::Color::Red;
			else if (map[i][j].closed) 	color = sf::Color::Green;
			else if (map[i][j].open) 	color = sf::Color::Yellow;
			else 						color = sf::Color::White;

			shape.setFillColor(color);
			w.draw(shape);
		}
	}
	// drawing a path if the Astar algorithm was successful
	if (pathFound) {
	// we will implement this part at a later stage.
		sf::Vector2i current = goal;
		while (current != start) {

			shape.setPosition(current.x * tileSize,current.y*tileSize);
			shape.setFillColor(sf::Color::Blue);
			w.draw(shape);

		//	sf::sleep(sf::milliseconds(10));
		//	std::cout << current.x << " " << current.y;
		//	std::cout << tile(current).parent.x << " " << tile(current).parent.y;

			current = tile(current).parent;
			//current = start;
		}
	}
}

void TileMap::restartAstar(){

	// here you should set closed and open to be false on all tiles
	for(int i = 0; i < (NUM_OF_TILES + 2); i++){
		for(int j = 0; j < (NUM_OF_TILES + 2); j++){
			map[i][j].open = false;
			map[i][j].closed = false;
			map[i][j].g = 99999;
			map[i][j].f = 999999;
		}
	}

	// we empty the openList
	openList.clear();
	// we have not found a solution yet
	pathFound = false;

}
