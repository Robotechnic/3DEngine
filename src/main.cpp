#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "math/vector3.hpp"
#include "shapes/cube.hpp"
#include "scene/scene.hpp"

#define CUBES 4

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "3D render");
	Scene scene(500, 500, 90, 1, 1000);
	scene.wireframe = true;
	scene.normals = true;
	scene.faces = false;
	scene.setCamera(Vector3f(0, 0, -100), Vector3f(0, 0, 0));

	window.setFramerateLimit(60);
	std::vector<Cube> cubes(CUBES);

	Vector3f cubePos[4] = {
		Vector3f(0,0,0),
		Vector3f(50,0,0),
		Vector3f(0,50,0),
		Vector3f(-50,0,0)
	};

	for (int i = 0; i < CUBES; i++) {
		cubes.at(i).setSize(20,20,20);
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
                window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			scene.cameraPosition.y += 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			scene.cameraPosition.y -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			scene.cameraPosition.x -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			scene.cameraPosition.x += 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			scene.cameraPosition.z += 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			scene.cameraPosition.z -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].rotate(0, M_PI/60, 0);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			for (int i = 0; i < CUBES; i++) {
				cubes[i].rotate(0, -M_PI/60, 0);
			}
		}


		window.clear();
		scene.clear();

		for (int i = 0; i < CUBES; i++) {
			scene.pushMatrix();
			scene.translate(cubePos[i]);
			scene.drawShape(&cubes[i]);
			scene.popMatrix();
		}



		window.draw(scene);
		window.display();
	}

	return 0;
}