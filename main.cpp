#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "Map.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "A*");
	TileMap tileMap;
	sf::Clock clock;
	clock.restart();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed){
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				// adjust to indices on tileMap
				if(mousePos.x <= ((NUM_OF_TILES) * tileMap.tileSize) && mousePos.x > tileMap.tileSize &&
					mousePos.y <= ((NUM_OF_TILES) * tileMap.tileSize) && mousePos.y > tileMap.tileSize)
				{
					mousePos.x = (mousePos.x + tileMap.tileSize / 2) / tileMap.tileSize;
					mousePos.y = (mousePos.y + tileMap.tileSize / 2) / tileMap.tileSize;

					// This implementation allows for changing tiles outside the
					// interior of the tileMap. Rewrite so that only the interior
					// can be changed.

					tileMap.tile(mousePos).traversable = !tileMap.tile(mousePos).traversable;
				}

			}
		}

		float dt = clock.restart().asSeconds();
		tileMap.update();
        window.clear();
		tileMap.draw(window);
        window.display();
    }

    return 0;
}
