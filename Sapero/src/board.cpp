#include <random>

#include <board.h>


namespace saper {


float CBoard::MAP_OFFSET_X = 10.0f;
float CBoard::MAP_OFFSET_Y = 50.0f;
float CBoard::MAP_CELL_SIZE = 16.0f;

CBoard::CBoard(unsigned int height, unsigned int width, int bombs)
		: height(height),
		  width(width),
		  bombs(bombs),
		  playerLost(false),
		  playerWin(false) {

	init();
}

CBoard::~CBoard() {
}

bool CBoard::init() {
	// create font
	if (!font.loadFromFile("../asset/courbd.ttf")) {
		return false;
	}

	// set title
	title.setFont(font);
	title.setPosition(10, 0);

	// create textures
	for (int i = CELL_TYPE::CT_VALUE_0; i < CELL_TYPE::CT_SIZE; i++) {
		mapTexType[(CELL_TYPE)i] = sf::Texture();
	}

	if ( !mapTexType[CELL_TYPE::CT_VALUE_0].loadFromFile("../asset/value_0.png") ||
		 !mapTexType[CELL_TYPE::CT_VALUE_1].loadFromFile("../asset/value_1.png") ||
		 !mapTexType[CELL_TYPE::CT_VALUE_2].loadFromFile("../asset/value_2.png") ||
		 !mapTexType[CELL_TYPE::CT_VALUE_3].loadFromFile("../asset/value_3.png") ||
		 !mapTexType[CELL_TYPE::CT_VALUE_4].loadFromFile("../asset/value_4.png") ||
		 !mapTexType[CELL_TYPE::CT_VALUE_5].loadFromFile("../asset/value_5.png") ||
		 !mapTexType[CELL_TYPE::CT_VALUE_6].loadFromFile("../asset/value_6.png") ||
		 !mapTexType[CELL_TYPE::CT_VALUE_7].loadFromFile("../asset/value_7.png") ||
 		 !mapTexType[CELL_TYPE::CT_VALUE_8].loadFromFile("../asset/value_8.png") ||
		 !mapTexType[CELL_TYPE::CT_BOMB].loadFromFile("../asset/bomb.png") ||
		 !mapTexType[CELL_TYPE::CT_NO_BOOM].loadFromFile("../asset/no_bomb.png") ||
		 !mapTexType[CELL_TYPE::CT_BOOM].loadFromFile("../asset/boom.png") ) {
		title.setString("Error: Load type texures faild!");
		return false;
	}
	
	for (int i = CELL_MODE::CM_SHOW; i < CELL_MODE::CM_SIZE; i++) {
		mapTexMode[(CELL_MODE)i] = sf::Texture();
	}

	if ( !mapTexMode[CELL_MODE::CM_HIDE].loadFromFile("../asset/hide.png") ||
		 !mapTexMode[CELL_MODE::CM_QUESTION].loadFromFile("../asset/question.png") ||
		 !mapTexMode[CELL_MODE::CM_FLAG].loadFromFile("../asset/flag.png") ) {
		title.setString("Error: Load mode texures faild!");
		return false;
	}

	// resize bombs
	vec2d.resize(height);
	for (unsigned int i = 0; i < vec2d.size(); ++i) {
		vec2d[i].resize(width);
	}

	// rand bombs
	for (int i = 0; i < bombs; ++i) {
		sf::Vector2u idxRand = CBoard::randomXY();
		if (vec2d[idxRand.x][idxRand.y].type != CELL_TYPE::CT_BOMB) {
			vec2d[idxRand.x][idxRand.y].type = CELL_TYPE::CT_BOMB;
		} else {
			--i;
		}
	}

	// fill cells
	for (unsigned int i = 0; i < vec2d.size(); ++i) {
		for (unsigned int j = 0; j < vec2d[i].size(); ++j) {
			CCell& cell = vec2d[i][j];
			cell.mode = CELL_MODE::CM_HIDE;
			cell.type = calcType(i, j);
			cell.sprite.setPosition(MAP_OFFSET_X + i * MAP_CELL_SIZE, MAP_OFFSET_Y + j * MAP_CELL_SIZE);
		}
	}
	
	update();

	return true;
}

bool CBoard::input(sf::Vector2i pos, sf::Mouse::Button button) {
	if (playerLost || playerWin) {
		return false;
	}

	int x = pos.x - static_cast<int>(MAP_OFFSET_X);
	int y = pos.y - static_cast<int>(MAP_OFFSET_Y);
	x /= static_cast<int>(MAP_CELL_SIZE);
	y /= static_cast<int>(MAP_CELL_SIZE);
	
	if (x < 0 || x >= height || y < 0 || y >= width) {
		return false;
	}

	if (button == sf::Mouse::Button::Right) {
		select(x, y);
	} else if (button == sf::Mouse::Button::Left) {
		disarm_r(x, y);
	}
	
	checkEnd();
	update();

	return true;
}

bool CBoard::update() {
	if (playerWin) {
		title.setString("Well Done!");
	} else if (playerLost) {
		title.setString("Game Over!");
	} else {
		title.setString("Bombs: " + std::to_string(bombs));
	}

	for (unsigned int i = 0; i < vec2d.size(); ++i) {
		for (unsigned int j = 0; j < vec2d[i].size(); ++j) {
			CCell& cell = vec2d[i][j];
			if (cell.mode == CELL_MODE::CM_SHOW) {
				cell.sprite.setTexture(mapTexType[cell.type]);
			} else {
				cell.sprite.setTexture(mapTexMode[cell.mode]);
			}
			// for test
			//cell.sprite.setTexture(mapTexType[CELL_TYPE::CT_VALUE_2]);
		}
	}
	return true;
}


bool CBoard::draw(sf::RenderWindow& window)  {
	window.draw(title);
	for (unsigned int i = 0; i < vec2d.size(); ++i) {
		for (unsigned int j = 0; j < vec2d[i].size(); ++j) {
			CCell &cell = vec2d[i][j];
			window.draw(cell.sprite);
		}
	}
	return true;
}

bool CBoard::select(unsigned int x, unsigned int y) {
	CCell& cell = vec2d[x][y];

	if (cell.mode == CELL_MODE::CM_HIDE) {
		cell.mode = CELL_MODE::CM_FLAG;
		--bombs;
	}
	else if (cell.mode == CELL_MODE::CM_FLAG) {
		cell.mode = CELL_MODE::CM_QUESTION;
		++bombs;
	}
	else if (cell.mode == CELL_MODE::CM_QUESTION) {
		cell.mode = CELL_MODE::CM_HIDE;
	}
	return true;
}

bool CBoard::disarm_r(unsigned int x, unsigned int y) {
	CCell& cell = vec2d[x][y];

	if (cell.mode != CELL_MODE::CM_HIDE) {
		return false;
	}

	cell.mode = CELL_MODE::CM_SHOW;

	if (cell.type == CELL_TYPE::CT_VALUE_0) {
		sf::Vector2u rangeX = calcBorder(x, height - 1);
		sf::Vector2u rangeY = calcBorder(y, width - 1);

		bool bRet = false;
		for (unsigned int i = rangeX.x; i <= rangeX.y; ++i) {
			for (unsigned int j = rangeY.x; j <= rangeY.y; ++j) {
				bRet |= disarm_r(i, j);
			}
		}
		return bRet;
	}

	return true;
}


bool CBoard::checkEnd() {
	unsigned int count = 0;
	for (unsigned int i = 0; i < vec2d.size(); ++i) {
		for (unsigned int j = 0; j < vec2d[i].size(); ++j) {
			CCell &cell = vec2d[i][j];
			if (cell.mode == CELL_MODE::CM_SHOW) {
				if (cell.type == CELL_TYPE::CT_BOMB) {
					cell.type = CELL_TYPE::CT_BOOM;
					playerLost = true;
				} else {
					++count;
				}
			} if (cell.mode == CELL_MODE::CM_FLAG && cell.type == CELL_TYPE::CT_BOMB ) {
				++count;
			}
		}

		if (playerLost) {
			break;
		}
	}

	if (playerLost) {
		// show bad choice and hidden bombs
		for (unsigned int i = 0; i < vec2d.size(); ++i) {
			for (unsigned int j = 0; j < vec2d[i].size(); ++j) {
				CCell &cell = vec2d[i][j];
				if (cell.mode == CELL_MODE::CM_FLAG && cell.type != CELL_TYPE::CT_BOMB) {
					cell.mode = CELL_MODE::CM_SHOW;
					cell.type = CELL_TYPE::CT_NO_BOOM;
				}
				else if (cell.mode == CELL_MODE::CM_HIDE &&  cell.type == CELL_TYPE::CT_BOMB) {
					cell.mode = CELL_MODE::CM_SHOW;
				}
			}
		}
		return true;
	}

	if (count == width * height) {
		playerWin = true;
	}

	return true;
}

sf::Vector2u CBoard::calcBorder(unsigned int idx, unsigned int maxRange) const {
	unsigned int min = 0;
	unsigned int max = 0;

	if (idx == 0) {
		min = idx;
		max = idx + 1;
	}
	else if (idx == maxRange) {
		min = idx - 1;
		max = idx;
	}
	else {
		min = idx - 1;
		max = idx + 1;
	}
	return sf::Vector2u(min, max);
}

CELL_TYPE CBoard::calcType(unsigned int x, unsigned int y) const {
	if (vec2d[x][y].type == CELL_TYPE::CT_BOMB) {
		return vec2d[x][y].type;
	}

	int type = CELL_TYPE::CT_VALUE_0;
	sf::Vector2u rangeX = calcBorder(x, height - 1);
	sf::Vector2u rangeY = calcBorder(y, width - 1);

	for (unsigned int i = rangeX.x; i <= rangeX.y; ++i) {
		for (unsigned int j = rangeY.x; j <= rangeY.y; ++j) {
			if (vec2d[i][j].type == CELL_TYPE::CT_BOMB) {
				type++;
			}
		}
	}

	return static_cast<CELL_TYPE>(type);
}

sf::Vector2u CBoard::randomXY() const {
	std::mt19937 rng;
	rng.seed(std::random_device()());
	
	std::uniform_int_distribution<unsigned int> distributionX(0, height - 1);
	std::uniform_int_distribution<unsigned int> distributionY(0, width - 1);

	return sf::Vector2u(distributionX(rng), distributionY(rng));
}

}