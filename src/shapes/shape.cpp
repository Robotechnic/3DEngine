#include "shapes/shape.hpp"

Shape::Shape(Vector3f size) : 
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

/**
 * @brief set the object size on each axis
 * 
 * @param size size of the object on each axis 1 = 100%
 */
void Shape::setSize(Vector3f size) {
	this->size = size;
	this->updateNeeded = true;
	this->update();
}

/**
 * @brief set the object size on each axis
 * 
 * @see Shape::setSize(Vector3f)
 * 
 * @param x the size of the object on the x axis
 * @param y the size of the object on the y axis
 * @param z the size of the object on the z axis
 */
void Shape::setSize(float x, float y, float z) {
	this->setSize(Vector3f(x, y, z));
}

/**
 * @brief return the object size on each axis
 * 
 * @return Vector3f the size of the object on each axis
 */
Vector3f Shape::getSize() {
	return this->size;
}

/**
 * @brief set the object rotation on each axis
 * 
 * @param rotation the rotation to apply
 */
void Shape::setRotation(Vector3f rotation) {
	this->rotation = rotation;
	this->rotationMatrix = Matrix4::rotation(rotation);
	this->updateNeeded = true;
	this->update();
}

/**
 * @brief set the object rotation on each axis
 * 
 * @see Shape::setRotation(Vector3f)
 * 
 * @param x the rotation to apply on the x axis
 * @param y the rotation to apply on the y axis
 * @param z the rotation to apply on the z axis
 */
void Shape::setRotation(float x, float y, float z) {
	this->setRotation(Vector3f(x, y, z));
}

/**
 * @brief return the object rotation on each axis
 * 
 * @return Vector3f the rotation of the object on each axis
 */
Vector3f Shape::getRotation() {
	return this->rotation;
}

/**
 * @brief rotate the object by a given angle on each axis, rotation is added to the current rotation
 * 
 * @see Shape::setRotation(Vector3f)
 * 
 * @param rotation the rotation to apply
 */
void Shape::rotate(Vector3f rotation) {
	this->rotation += rotation;
	this->rotationMatrix = Matrix4::rotation(this->rotation);
	this->updateNeeded = true;
	this->update();
}

/**
 * @brief rotate the object by a given angle on each axis, rotation is added to the current rotation
 * 
 * @see Shape::rotate(Vector3f)
 * 
 * @param x the rotation to apply on the x axis
 * @param y the rotation to apply on the y axis
 * @param z the rotation to apply on the z axis
 */
void Shape::rotate(float x, float y, float z) {
	this->rotate(Vector3f(x, y, z));
}

/**
 * @brief initialise the shapes triangles
 * 
 */
void Shape::init() {
	this->shape_init();
	this->updateNeeded = false;
}

/**
 * @brief update the shapes triangles if needed
 * 
 */
void Shape::update() {
	if (!this->updateNeeded)
		return
	this->shape_update();
	this->updateNeeded = false;
}