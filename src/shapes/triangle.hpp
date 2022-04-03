#pragma once

#include <stdexcept>
#include "math/vector3.hpp"

class Triangle {
	public:
		Triangle(Vector3f v1, Vector3f v2, Vector3f v3);
		Triangle(const Triangle& other);

		void calculateNormal();

		Vector3f at(float index) const;

		Vector3f& operator()(float index);

		Vector3f getCenter() const;
		Vector3f getNormal() const;

		Vector3f v1;
		Vector3f v2;
		Vector3f v3;
	private:
		Vector3f normal;
};