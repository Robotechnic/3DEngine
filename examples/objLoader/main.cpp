#include <SFML/Graphics.hpp>
#include <iostream>
#include "math/vector3.hpp"
#include "shapes/objloader.hpp"
#include "scene/scene.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "OBJ Loader");
	Scene scene(500, 500, 90, 1, 1000);
	scene.wireframe = false;
	scene.normals = false;
	scene.normalLength = 0.1;
	scene.faces = true;

	scene.setCamera(Vector3f(0, 4, -3), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

	window.setFramerateLimit(60);

	ObjLoader loader;
	loader.loadObjFile("./assets/teapot.obj");

	if (!loader.isLoaded()) {
		std::cout << "Error loading file" << std::endl;
		std::cout << loader.getErrorMessage() << std::endl;
		std::cout << "At line: " << loader.getErrorLine() << std::endl;
		
		return 1;
	}
	std::cout << "Object loaded" <<std::endl;

	float rotation = 0, distance = 1;
	float direction = 0.02;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                window.close();
			}
		}

		rotation += 0.01;
		distance += direction;

		if (rotation > 2 * 3.1415) {
			rotation = 0;
		}

		if (distance < -1) {
			direction *= -1;
		} else if (distance > 2) {
			direction *= -1;
		}

		scene.clear();
		scene.pushMatrix();
		scene.translate(0, 0, distance);
		scene.rotate(0, rotation, 0);
		scene.drawShape(&loader);
		scene.popMatrix();

		window.clear();
		scene.draw(window);
		window.display();
	}

	return 0;
}