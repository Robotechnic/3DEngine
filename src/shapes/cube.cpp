#include "cube.hpp"

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
	Shape(size, pos),
	color(sf::Color::White)
{
	this->init();
}

void Cube::init() {
	for (int i = 0; i < 36; i += 3) {
		Triangle *t = new Triangle();
		t->v1 = this->rotationMatrix * (vertex_pos[i] * size) + pos;
		t->v2 = this->rotationMatrix * (vertex_pos[i + 1] * size) + pos;
		t->v3 = this->rotationMatrix * (vertex_pos[i + 2] * size) + pos;
		this->triangles.push_back(t);
	}

	for (int i = 0; i < 36; i++) {
		this->colors.push_back(new sf::Color(this->color));
	}
	this->updateNeeded = false;
}

void Cube::update() {
	if (!this->updateNeeded) return;
	for (int i = 0; i < 36; i += 3) {
		Triangle *t = this->triangles[i / 3];
		t->v1 = this->rotationMatrix * (vertex_pos[i] * size) + pos;
		t->v2 = this->rotationMatrix * (vertex_pos[i + 1] * size) + pos;
		t->v3 = this->rotationMatrix * (vertex_pos[i + 2] * size) + pos;
	}

	for (sf::Color *c : this->colors) {
		*c = this->color;
	}
	this->updateNeeded = false;
}