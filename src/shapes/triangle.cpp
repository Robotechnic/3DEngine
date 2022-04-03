#include "triangle.hpp"

Triangle::Triangle(Vector3f v1, Vector3f v2, Vector3f v3) :
	v1(v1),
	v2(v2),
	v3(v3)
{
	this->calculateNormal();
}

Triangle::Triangle(const Triangle& other) : 
	v1(other.v1),
	v2(other.v2),
	v3(other.v3),
	normal(other.normal)
{}

void Triangle::calculateNormal() {
	normal = (this->v2 - this->v1).cross(this->v3 - this->v1);
	normal.normalize();
}

Vector3f Triangle::at(float index) const {
	if (index == 0) {
		return this->v1;
	} else if (index == 1) {
		return this->v2;
	} else if (index == 2) {
		return this->v3;
	} else {
		throw std::runtime_error("Index out of range");
	}
}

Vector3f& Triangle::operator()(float index) {
	if (index == 0) {
		return this->v1;
	} else if (index == 1) {
		return this->v2;
	} else if (index == 2) {
		return this->v3;
	} else {
		throw std::runtime_error("Index out of range");
	}
}

Vector3f Triangle::getCenter() const {
	return (this->v1 + this->v2 + this->v3) / 3.0f;
}

Vector3f Triangle::getNormal() const {
	return this->normal;
}