#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <stdexcept>
#include "math/vector3.hpp"
#include "math/matrix4.hpp"
#include "shapes/shape.hpp"

class Scene : public sf::Drawable {
	public:
		Scene(float width, float height, float fov, float near, float far);

		// void translate(const Vector3<float>& translation);
		// void rotate(const Vector3<float>& rotation);

		void removeNullTriangles();
		void update();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void addShape(Shape *shape);

		bool wireframe;
	
	private:
		float width, height;
		Matrix4 projectionMatrix;
		std::vector <Triangle *> triangles;
		std::vector <sf::Color *> colors;

};