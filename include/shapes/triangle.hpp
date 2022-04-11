#pragma once

#include <stdexcept>
#include <tuple>
#include "math/vector3.hpp"
#include "math/matrix4.hpp"

class Triangle {
	public:
		Triangle(Vector3f v1 = Vector3f(), Vector3f v2 = Vector3f(), Vector3f v3 = Vector3f(), Vector3f normal = Vector3f());
		Triangle(const Triangle& other);

		void calculateNormal();

		Vector3f at(unsigned index) const;

		Vector3f& operator()(unsigned index);

		Vector3f getCenter() const;
		Vector3f getNormal() const;

		std::pair<Vector3f, Vector3f> getLeftRightIntersection(
			const Vector3f &planeNormal, 
			const float &planeD,
			unsigned index
		) const;

		std::pair<int, int> getDistancesToPlane(
			const Vector3f &planeNormal, 
			const float &planeD
		) const;

		void applyTransform(const Matrix4 &rotation, const Vector3f &translation);

		Vector3f v1;
		Vector3f v2;
		Vector3f v3;
		Vector3f normal;
};