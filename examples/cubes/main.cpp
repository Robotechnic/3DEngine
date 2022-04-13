#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "math/vector3.hpp"
#include "shapes/cube.hpp"
#include "scene/scene.hpp"

#define CUBES 4

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "3D render");
	Scene scene(500, 500, 90, 1, 1000);
	scene.wireframe = false;
	scene.normals = false;
	scene.faces = true;
	scene.setCamera(Vector3f(0, 0, -100), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

	window.setFramerateLimit(60);

	Vector3f cubePos[4] = {
		Vector3f(0,0,0),
		Vector3f(65,0,0),
		Vector3f(0,65,0),
		Vector3f(-65,0,0)
	};

	sf::Color facesColors[6] = {
		sf::Color::Red,
		sf::Color::Blue,
		sf::Color::Cyan,
		sf::Color::Magenta,
		sf::Color::Green,
		sf::Color::Yellow
	};

	std::vector<Cube> displayCubes(CUBES);
	for (int i = 0; i < CUBES; i++) {
		displayCubes.at(i).setSize(25, 25, 25);
		displayCubes.at(i).setFacesColors(facesColors);
	}

	float rotation = 0;
	float distance = 250;

	float cubeRotation = 0;

	sf::Clock clock;
	float fps = 0;

	while (window.isOpen()) {
		clock.restart();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                window.close();
			} else if (event.type == sf::Event::Resized){
				scene.resize(event.size.width, event.size.height);	
			} else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::W) {
					scene.wireframe = !scene.wireframe;
				} else if (event.key.code == sf::Keyboard::N) {
					scene.normals = !scene.normals;
				} else if (event.key.code == sf::Keyboard::F) {
					scene.faces = !scene.faces;
				} else if (event.key.code == sf::Keyboard::B) {
					scene.zbuffer = !scene.zbuffer;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			distance -= 1;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			distance += 1;
		}

		rotation += 0.01;
		cubeRotation += 0.01;

		scene.setCamera(Vector3f(cos(rotation) * distance, 50, sin(rotation) * distance), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
		
		if(rotation > 2 * 3.1415926536) {
			rotation = 0;
		}
		if (cubeRotation >  2 * 3.1415926536) {
			cubeRotation = 0;
		}

		window.clear();
		scene.clear();

		for (int i = 0; i < CUBES; i++) {
			scene.pushMatrix();
			scene.translate(cubePos[i]);
			scene.rotate(0, cubeRotation, 0);
			scene.drawShape(&displayCubes[i]);
			scene.popMatrix();
		}

		scene.draw(window);
		window.display();
	}

	return 0;
}