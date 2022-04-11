#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include "math/vector3.hpp"
#include "math/matrix4.hpp"
#include "shapes/triangle.hpp"

class Shape {
	public:
		Shape(Vector3f size);
		Shape(const Shape& other);
		~Shape();

		void init();
		void update();

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

		virtual void shape_init() = 0;
		virtual void shape_update() = 0;
};