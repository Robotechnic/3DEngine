#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include "math/vector3.hpp"
#include "math/matrix4.hpp"

struct Triangle {
	Vector3f v1;
	Vector3f v2;
	Vector3f v3;
	Vector3f normal;

	void calculateNormal() {
		normal = (v2 - v1).cross(v3 - v1);
		normal.normalize();
	}

	Vector3f at(float index) const {
		if (index == 0) {
			return v1;
		} else if (index == 1) {
			return v2;
		} else if (index == 2) {
			return v3;
		} else {
			throw std::runtime_error("Index out of range");
		}
	}

	Vector3f& operator()(float index) {
		if (index == 0) {
			return v1;
		} else if (index == 1) {
			return v2;
		} else if (index == 2) {
			return v3;
		} else {
			throw std::runtime_error("Index out of range");
		}
	}

	Vector3f getCenter() const {
		return (v1 + v2 + v3) / 3.0f;
	}
};


class Shape {
	public:
		Shape(Vector3f size, Vector3f pos);
		Shape(const Shape& other);
		~Shape();

		virtual void init() = 0;
		virtual void update() = 0;

		virtual std::vector<Triangle> getTriangles() {
			this->update();
			return this->triangles;
		};
		virtual std::vector<sf::Color> getColors() {
			this->update();
			return this->colors;
		};

		void setSize(Vector3f size);
		void setSize(float x, float y, float z);
		Vector3f getSize();

		void setRotation(Vector3f rotation);
		void setRotation(float x, float y, float z);
		Vector3f getRotation();

		void rotate(Vector3f rotation);
		void rotate(float x, float y, float z);

	protected:
		Vector3f size, rotation;
		Matrix4 rotationMatrix;
		bool updateNeeded; //avoid unnecessary updates
		std::vector<Triangle> triangles;
		std::vector<sf::Color> colors;
};