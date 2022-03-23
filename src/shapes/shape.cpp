#include "shape.hpp"

Shape::Shape(Vector3f size, Vector3f pos) : size(size), pos(pos), rotation(0, 0, 0) {}
Shape::Shape(const Shape& other) : size(other.size), pos(other.pos), rotation(other.rotation) {}

void Shape::setPosition(Vector3f pos) {
	this->pos = pos;
}

void Shape::setPosition(float x, float y, float z) {
	this->setPosition(Vector3f(x, y, z));
}

Vector3f Shape::getPosition() {
	return this->pos;
}

void Shape::setSize(Vector3f size) {
	this->size = size;
}

void Shape::setSize(float x, float y, float z) {
	this->setSize(Vector3f(x, y, z));
}

Vector3f Shape::getSize() {
	return this->size;
}

void Shape::moove(Vector3f delta) {
	this->pos += delta;
}

void Shape::moove(float x, float y, float z) {
	this->moove(Vector3f(x, y, z));
}

void Shape::setRotation(Vector3f rotation) {
	this->rotation = rotation;
}

void Shape::setRotation(float x, float y, float z) {
	this->setRotation(Vector3f(x, y, z));
}

Vector3f Shape::getRotation() {
	return this->rotation;
}