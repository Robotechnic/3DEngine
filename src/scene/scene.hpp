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
		void setCamera(const Vector3f position, const Vector3f lookat);

		void removeNullTriangles();
		void update();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void addShape(Shape *shape);

		bool wireframe;
		bool normals;
		bool faces;
	
	private:
		bool isVisible(const Triangle &triangle) const;
		sf::VertexArray drawFaces() const;
		sf::VertexArray drawWireframe() const;
		sf::VertexArray drawNormals() const;

		sf::Vector2f getProjection(Vector3f vector) const;

		float width, height;
		Matrix4 projectionMatrix;

		Vector3f cameraPosition, cameraLookat;

		std::vector <Triangle *> triangles;
		std::vector <sf::Color *> colors;

};