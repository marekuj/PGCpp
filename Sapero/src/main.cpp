
#include <SFML/Graphics.hpp>
#include <board.h>

int main () {
	sf::RenderWindow window(sf::VideoMode(800, 640), "Sapero", sf::Style::Titlebar | sf::Style::Close);
	
	// height
	// width
	// bombs
	saper::CBoard board(18, 18, 22);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				board.input(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), event.mouseButton.button);
			}
		}
		window.clear(sf::Color(150, 150, 150, 255));
		board.draw(window);
		window.display();
	}

	return 0;
};