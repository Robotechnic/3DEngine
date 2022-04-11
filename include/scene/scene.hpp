#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <stack>
#include <stdexcept>
#include <limits>
#include <iostream>
#include "math/vector3.hpp"
#include "math/matrix4.hpp"
#include "shapes/shape.hpp"

class Scene {
	public:
		Scene(unsigned width, unsigned height, float fov, float near, float far);
		~Scene();

		void resize(unsigned width, unsigned height);
		void setFov(float fov);

		void setCamera(const Vector3f position, const Vector3f lookat, const Vector3f up);
		void setCamera(const Vector3f position, const float theta, const float phi, const Vector3f up);

		void popMatrix();
		void pushMatrix();
		void translate(Vector3f translation);
		void translate(float x, float y, float z);
		void rotate(Vector3f rotation);
		void rotate(float x, float y, float z);

		void clear();
		void draw(sf::RenderTarget& target);
		
		
		void drawShape(Shape *shape);
		void rasterizeTriangle(const Triangle &t, const sf::Color& color);

		bool wireframe;
		bool normals;
		bool faces;
		bool zbuffer;
		float normalLength;
	
	private:
		void initPixelsBuffers();
		inline bool isVisible(const Triangle &triangle) const;
		void drawFaces();
		void drawZBuffer();
		sf::VertexArray drawWireframe() const;
		sf::VertexArray drawNormals() const;
		
		void setTrianglePosFromCamera(Triangle &triangle) const;
		sf::Vector2f getProjection(Vector3f vector) const;
		void clipAgainstPlane(const Vector3f &planeNormal, const float &planeD);
		void clipTriangle(
			const Triangle &triangle, const sf::Color &color,
			const Vector3f &planeNormal, const float &planeD,
			std::vector<Triangle> &renderTriangles, std::vector<sf::Color> &renderColors
		) const;

		float computeZIndex(float w1, float w2, float w3, Vector3f v1, Vector3f v2, Vector3f v3);

		void computeProjectionMatrix();
		void computeCameraLookAt();
		
		float edgeFunction(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3) const;

		unsigned int width, height;
		float fov, near, far;
		Matrix4 projectionMatrix;

		Vector3f cameraPosition, cameraLookAt, cameraUp;
		Matrix4 cameraLookAtMatrix;

		std::stack<Matrix4> transformations;
		Matrix4 worldStateMatrix;

		// triangles and colors associated with them
		std::vector <Triangle> triangles;
		std::vector <sf::Color> colors;

		// draw buffer
		sf::Uint8 *pixels;
		sf::Texture texture;
		sf::Sprite sprite;
		float *zBuffer;
};