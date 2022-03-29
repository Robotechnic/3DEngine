#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "math/vector3.hpp"
#include "shapes/cube.hpp"
#include "scene/scene.hpp"

#define CUBES 4

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "3D render");
	Scene scene(500, 500, 90, 1, 100);
	scene.wireframe = false;
	scene.normals = false;
	scene.faces = true;
	scene.setCamera(Vector3f(0, 0, -100), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

	window.setFramerateLimit(60);
	std::vector<Cube> cubes(CUBES);

	Vector3f cubePos[4] = {
		Vector3f(0,0,0),
		Vector3f(50,0,0),
		Vector3f(0,50,0),
		Vector3f(-50,0,0)
	};

	sf::Color facesColors[6] = {
		sf::Color::Red,
		sf::Color::Blue,
		sf::Color::Cyan,
		sf::Color::Magenta,
		sf::Color::Green,
		sf::Color::Yellow
	};

	for (int i = 0; i < CUBES; i++) {
		cubes.at(i).setSize(20,20,20);
		cubes.at(i).setFacesColors(facesColors);
	}

	float rotation = 0;
	float distance = 250;

	float cubeRotation = 0;

	while (window.isOpen()) {
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

		scene.setCamera(Vector3f(cos(rotation) * distance, 50, sin(rotation) * distance), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
		
		rotation += 0.01;
		cubeRotation += 0.01;

		
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
			scene.rotate(cubeRotation, cubeRotation, 0);
			scene.drawShape(&cubes[i]);
			scene.popMatrix();
		}

		scene.draw(window);
		window.display();
	}

	return 0;
}