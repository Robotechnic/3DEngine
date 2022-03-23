#include "cube.h"

// store all cube vertex in triangle shape
const Vector3f vertex_pos[] {
	//front
	Vector3f(-1, -1, 1), Vector3f(1, -1, 1), Vector3f(1, 1, 1),
	Vector3f(-1, -1, 1), Vector3f(1, 1, 1), Vector3f(-1, 1, 1),

	// back
	Vector3f(-1, -1, -1), Vector3f(1, -1, -1), Vector3f(1, 1, -1),
	Vector3f(-1, -1, -1), Vector3f(1, 1, -1), Vector3f(-1, 1, -1),

	// left
	Vector3f(-1, -1, -1), Vector3f(-1, -1, 1), Vector3f(-1, 1, 1),
	Vector3f(-1, -1, -1), Vector3f(-1, 1, 1), Vector3f(-1, 1, -1),

	// right
	Vector3f(1, -1, -1), Vector3f(1, -1, 1), Vector3f(1, 1, 1),
	Vector3f(1, -1, -1), Vector3f(1, 1, 1), Vector3f(1, 1, -1),

	// top
	Vector3f(-1, 1, -1), Vector3f(1, 1, -1), Vector3f(1, 1, 1),
	Vector3f(-1, 1, -1), Vector3f(1, 1, 1), Vector3f(-1, 1, 1),

	// bottom
	Vector3f(-1, -1, -1), Vector3f(1, -1, -1), Vector3f(1, -1, 1),
	Vector3f(-1, -1, -1), Vector3f(1, -1, 1), Vector3f(-1, -1, 1)
};

Cube::Cube(Vector3f size, Vector3f pos) : 
	Shape(size, pos)
	{}

std::vector<Triangle> Cube::getTriangles() const {
	std::vector<Triangle> vertex;
	for (int i = 0; i < 36; i += 3) {
		Triangle t;
		t.v1 = vertex_pos[i] * size + pos;
		t.v2 = vertex_pos[i + 1] * size + pos;
		t.v3 = vertex_pos[i + 2] * size + pos;
		vertex.push_back(t);
	}
	return vertex;
}

std::vector<sf::Color> Cube::getColors() const {
	std::vector<sf::Color> color;
	for (int i = 0; i < 36; i++) {
		color.push_back(sf::Color::White);
	}
	return color;
}