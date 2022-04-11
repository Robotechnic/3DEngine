#include "shapes/triangle.hpp"

Triangle::Triangle(Vector3f v1, Vector3f v2, Vector3f v3, Vector3f normal) :
	v1(v1),
	v2(v2),
	v3(v3),
	normal(normal)
{
}

Triangle::Triangle(const Triangle& other) : 
	v1(other.v1),
	v2(other.v2),
	v3(other.v3),
	normal(other.normal)
{}

/**
 * @brief compute the triangle normal
 * 
 */
void Triangle::calculateNormal() {
	normal = (this->v2 - this->v1).cross(this->v3 - this->v1);
	normal.normalize();
}

Vector3f Triangle::at(unsigned index) const {
	if (index == 0) {
		return this->v1;
	} else if (index == 1) {
		return this->v2;
	} else if (index == 2) {
		return this->v3;
	}
	
	throw std::runtime_error("Index out of range");
	
}

Vector3f& Triangle::operator()(unsigned index) {
	if (index == 0) {
		return this->v1;
	} else if (index == 1) {
		return this->v2;
	} else if (index == 2) {
		return this->v3;
	}
	
	throw std::runtime_error("Index out of range");
}

/**
 * @brief return the center point of the triangle
 * 
 * @return Vector3f the triangle center point
 */
Vector3f Triangle::getCenter() const {
	return (this->v1 + this->v2 + this->v3) / 3.0f;
}

/**
 * @brief return the triange normal
 * 
 * @return Vector3f the triangle normal
 */
Vector3f Triangle::getNormal() const {
	return this->normal;
}

/**
 * @brief get "left" and "right" point of plane triangle intersection
 * 
 * @param planeNormal the plane normal vector (a, b and c in ax + by + cz = 0)
 * @param planeD the plane d in the equation ax + by + cz + d = 0
 * @param index the index of the point which is alone in one side of the plan
 * @return std::pair<Vector3f, Vector3f> the left and right point of the intersection
 */
std::pair<Vector3f, Vector3f> Triangle::getLeftRightIntersection(
	const Vector3f &planeNormal,
	const float &planeD,
	unsigned index
) const {
	Vector3f leftPoint = Vector3f::segmentPlaneIntersection(
		planeNormal, planeD,
		this->at(index), this->at((index + 1) % 3)
	);
	Vector3f rightPoint = Vector3f::segmentPlaneIntersection(
		planeNormal, planeD,
		this->at(index), this->at((index + 2) % 3)
	);
	return std::make_pair(leftPoint, rightPoint);
}

/**
 * @brief get the number of points "inside" the plane and the index of the point alone in one side of the plane
 * 
 * @param planeNormal the plane normal vector (a, b and c in ax + by + cz = 0)
 * @param planeD the plane d in the equation ax + by + cz + d = 0
 * @return std::pair<int, int> the index of the point alone in one side of the plane and the number of points "inside" the plane
 */
std::pair<int, int> Triangle::getDistancesToPlane(
	const Vector3f &planeNormal, 
	const float &planeD
) const {
	Vector3f distances;
	int inside = 0;
	for (unsigned i = 0; i < 3; i++) {
		float distance = planeNormal.dot(this->at(i)) + planeD;
		distances(i) = distance;
		if (distance > 0) {
			inside++;
		}
	}

	int pointIndex = 0;
	while ((inside == 1 && distances(pointIndex) < 0) || (inside == 2 && distances(pointIndex) > 0)) {
		pointIndex ++;
	}

	return std::make_pair(pointIndex, inside);
}

/**
 * @brief function that apply a rotation and a scaling to the triangle
 * 
 * @param rotation the rotation matrix to apply
 * @param size the scaling factor to apply on each axis
 */
void Triangle::applyTransform(const Matrix4 &rotation, const Vector3f &size) {
	this->v1 = rotation * (this->v1 * size);
	this->v2 = rotation * (this->v2 * size);
	this->v3 = rotation * (this->v3 * size);
	this->normal = rotation * this->normal;
}