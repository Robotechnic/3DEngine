#include "shapes/cube.hpp"

// store all cube vertex in triangle shape
const Vector3f vertex_pos[] {
	//front
	Vector3f(-1, -1, 1), Vector3f(1, -1, 1), Vector3f(1, 1, 1),
	Vector3f(-1, -1, 1), Vector3f(1, 1, 1), Vector3f(-1, 1, 1),

	// back
	Vector3f(1, 1, -1), Vector3f(1, -1, -1), Vector3f(-1, -1, -1),
	Vector3f(-1, 1, -1), Vector3f(1, 1, -1), Vector3f(-1, -1, -1),

	// right
	Vector3f(-1, -1, -1), Vector3f(-1, -1, 1), Vector3f(-1, 1, 1),
	Vector3f(-1, -1, -1), Vector3f(-1, 1, 1), Vector3f(-1, 1, -1),

	// left
	Vector3f(1, 1, 1), Vector3f(1, -1, 1), Vector3f(1, -1, -1),
	Vector3f(1, 1, -1), Vector3f(1, 1, 1), Vector3f(1, -1, -1),

	// top
	Vector3f(1, 1, 1), Vector3f(1, 1, -1), Vector3f(-1, 1, -1),
	Vector3f(-1, 1, 1), Vector3f(1, 1, 1), Vector3f(-1, 1, -1),

	// bottom
	Vector3f(-1, -1, -1), Vector3f(1, -1, -1), Vector3f(1, -1, 1),
	Vector3f(-1, -1, -1), Vector3f(1, -1, 1), Vector3f(-1, -1, 1)
};

Cube::Cube(Vector3f size) : 
	Shape(size),
	color(sf::Color::White)
{
	this->init();
}

void Cube::shape_init() {
	this->triangles.resize(12);
	for (int i = 0; i < 12; i ++) {
		this->triangles.at(i) = Triangle({
			this->rotationMatrix * (vertex_pos[i * 3] * size),
			this->rotationMatrix * (vertex_pos[i * 3 + 1] * size),
			this->rotationMatrix * (vertex_pos[i * 3 + 2] * size),
		});
		this->triangles.at(i).calculateNormal();
	}
	this->colors.resize(12, sf::Color(this->color));
}

void Cube::shape_update() {
	for (int i = 0; i < 36; i += 3) {
		Triangle *t = &this->triangles[i / 3];
		t->v1 = this->rotationMatrix * (vertex_pos[i] * size);
		t->v2 = this->rotationMatrix * (vertex_pos[i + 1] * size);
		t->v3 = this->rotationMatrix * (vertex_pos[i + 2] * size);
		t->calculateNormal();
	}
}

/**
 * @brief set the color of a specific face
 * 
 * @param face the id of the face in order front, back, right, left, top, bottom
 * @param color the color to set
 */
void Cube::setFaceColor(const unsigned face, const sf::Color color) {
	if (face > 5) {
		throw std::out_of_range("Face index out of range");
	}
	this->colors[face] = color;
}

/**
 * @brief set color of all faces
 * 
 * @param colors color to set
 */
void Cube::setFacesColors(const sf::Color colors[6]) {
	for (int j = 0; j < 12; j++) {
		this->colors.at(j) = colors[j/2];
	}
}