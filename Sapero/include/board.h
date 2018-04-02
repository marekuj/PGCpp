#pragma once

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include <cell.h>

namespace saper {

class CBoard {
public:
	CBoard(unsigned int height, unsigned int width, int bombs);
	~CBoard();

	bool init();
	bool input(sf::Vector2i pos, sf::Mouse::Button button);
	bool update();
	bool draw(sf::RenderWindow& window);
	bool releasa();

private:
	static float MAP_OFFSET_X;
	static float MAP_OFFSET_Y;
	static float MAP_CELL_SIZE;

	unsigned int height;
	unsigned int width;
	int bombs;
	bool isGameOver;

	sf::Font font;
	sf::Text title;

	std::map<CELL_TYPE, sf::Texture> mapTexType;
	std::map<CELL_MODE, sf::Texture> mapTexMode;
	std::vector<std::vector<CCell>> vec2d;
		
	// actions
	bool select(unsigned int x, unsigned int y);
	bool disarm_r(unsigned int x, unsigned int y);

	// tools
	bool checkGameOver();
	sf::Vector2u calcBorder(unsigned int idx, unsigned int maxRange) const;
	CELL_TYPE calcType(unsigned int x, unsigned int y) const;
	sf::Vector2u randomXY() const;
};

}

