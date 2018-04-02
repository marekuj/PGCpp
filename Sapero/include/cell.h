#pragma once

#include <SFML/Graphics.hpp>


namespace saper {

enum CELL_TYPE {
	CT_VALUE_0 = 0,
	CT_VALUE_1,
	CT_VALUE_2,
	CT_VALUE_3,
	CT_VALUE_4,
	CT_VALUE_5,
	CT_VALUE_6,
	CT_VALUE_7,
	CT_VALUE_8,
	CT_BOMB,
	CT_NO_BOOM,
	CT_BOOM,
	CT_SIZE
};

enum CELL_MODE {
	CM_SHOW,
	CM_HIDE,
	CM_QUESTION,
	CM_FLAG,		
	CM_SIZE
};

struct CCell {
	sf::Sprite sprite;
	CELL_TYPE type;
	CELL_MODE mode;
};

}
