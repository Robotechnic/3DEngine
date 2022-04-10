#include <SFML/Graphics.hpp>
#include <iostream>
#include "math/vector3.hpp"
#include "shapes/objloader.hpp"
#include "scene/scene.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(500, 500), "OBJ Loader");
	Scene scene(500, 500, 90, 1, 1000);
	scene.wireframe = true;
	scene.normals = false;
	scene.faces = false;

	window.setFramerateLimit(60);

	ObjLoader loader;
	loader.loadObjFile("./assets/teapot.obj");

	if (!loader.isLoaded()) {
		std::cout << "Error loading file" << std::endl;
		std::cout << loader.getErrorMessage() << std::endl;
		std::cout << "At line: " << loader.getErrorLine() << std::endl;
		
		return 1;
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                window.close();
			}
		}

		scene.drawShape(&loader);

		window.clear();
		scene.draw(window);
		window.display();
	}

	return 0;
}