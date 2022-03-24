#include "shape.hpp"

Shape::Shape(Vector3f size, Vector3f pos) : 
	size(size), 
	pos(pos), 
	rotation(0, 0, 0), 
	rotationMatrix(Matrix3::rotation(0, 0, 0)), 
	updateNeeded(true) 
{}

Shape::Shape(const Shape& other) : 
	size(other.size), 
	pos(other.pos), 
	rotation(other.rotation), 
	rotationMatrix(other.rotationMatrix),
	updateNeeded(true) 
{}

Shape::~Shape() {
	for (long unsigned int i = 0; i < this->triangles.size(); i++) {
		delete this->triangles[i];
	}
	this->triangles.clear();
	for (long unsigned int i = 0; i < this->colors.size(); i++) {
		delete this->colors[i];
	}
	this->colors.clear();
}

void Shape::setPosition(Vector3f pos) {
	this->pos = pos;
	this->updateNeeded = true;
	this->update();
}

void Shape::setPosition(float x, float y, float z) {
	this->setPosition(Vector3f(x, y, z));
}

Vector3f Shape::getPosition() {
	return this->pos;
}

void Shape::setSize(Vector3f size) {
	this->size = size;
	this->updateNeeded = true;
	this->update();
}

void Shape::setSize(float x, float y, float z) {
	this->setSize(Vector3f(x, y, z));
}

Vector3f Shape::getSize() {
	return this->size;
}

void Shape::moove(Vector3f delta) {
	this->pos += delta;
	this->updateNeeded = true;
	this->update();
}

void Shape::moove(float x, float y, float z) {
	this->moove(Vector3f(x, y, z));
}

void Shape::setRotation(Vector3f rotation) {
	this->rotation = rotation;
	this->rotationMatrix = Matrix3::rotation(rotation);
	this->updateNeeded = true;
	this->update();
}

void Shape::setRotation(float x, float y, float z) {
	this->setRotation(Vector3f(x, y, z));
}

Vector3f Shape::getRotation() {
	return this->rotation;
}

void Shape::rotate(Vector3f rotation) {
	this->rotation += rotation;
	this->rotationMatrix = Matrix3::rotation(this->rotation);
	this->updateNeeded = true;
	this->update();
}

void Shape::rotate(float x, float y, float z) {
	this->rotate(Vector3f(x, y, z));
}