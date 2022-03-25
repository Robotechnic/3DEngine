#include "shape.hpp"

Shape::Shape(Vector3f size, Vector3f pos) : 
	size(size),
	rotation(0, 0, 0), 
	rotationMatrix(Matrix4::rotation(0, 0, 0)), 
	updateNeeded(true) 
{}

Shape::Shape(const Shape& other) : 
	size(other.size),
	rotation(other.rotation), 
	rotationMatrix(other.rotationMatrix),
	updateNeeded(true) 
{}

Shape::~Shape() {
	this->triangles.clear();
	this->colors.clear();
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

void Shape::setRotation(Vector3f rotation) {
	this->rotation = rotation;
	this->rotationMatrix = Matrix4::rotation(rotation);
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
	this->rotationMatrix = Matrix4::rotation(this->rotation);
	this->updateNeeded = true;
	this->update();
}

void Shape::rotate(float x, float y, float z) {
	this->rotate(Vector3f(x, y, z));
}