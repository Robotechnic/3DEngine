#include <SFML/Graphics.hpp>
#include <iostream>
#include "shapes/cube.hpp"
#include "math/vector3.hpp"
#include "math/matrix3.hpp"

#define CUBES 1

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "3D render");
	window.setFramerateLimit(60);
	Cube cubes[4] = {
		Cube(Vector3f(50,50,50)),
		Cube(Vector3f(50,50,50)),
		Cube(Vector3f(50,50,50)),
		Cube(Vector3f(50,50,50))
	};

	cubes[0].setPosition(0,0,100);
	cubes[1].setPosition(50,0,100);
	cubes[2].setPosition(0,50,100);
	cubes[3].setPosition(-50,0,100);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
                window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].moove(0,-1,0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].moove(0,1,0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].moove(-1,0,0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].moove(1,0,0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].moove(0,0,1);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].moove(0,0,-1);
			}
		}

		window.clear();

		window.display();
	}

	return 0;
}