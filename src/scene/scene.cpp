#include "scene.hpp"

Scene::Scene(float width, float height, float fov, float near, float far) {
	this->projectionMatrix = Matrix4::projectionMatrix(fov, width / height, near, far);
	this->wireframe = false;
	this->width = width;
	this->height = height;
}

void Scene::addShape(Shape *shape) {
	std::vector <Triangle *> triangles = shape->getTriangles();
	std::vector <sf::Color *> colors = shape->getColors();

	if (triangles.size() * 3 != colors.size()) {
		throw std::runtime_error("triangles and colors size mismatch");
	}

	this->triangles.insert(this->triangles.end(), triangles.begin(), triangles.end());
	this->colors.insert(this->colors.end(), colors.begin(), colors.end());
}

void Scene::removeNullTriangles() {
	// if a shape is deleted, the triangles of the shape are deleted too
	// but the pointers to the triangles are still in the scene, so we need to remove them
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		if (this->triangles[i] == nullptr) {
			this->triangles.erase(this->triangles.begin() + i);
		}
	}
}

void Scene::update() {
	this->removeNullTriangles();
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::VertexArray vertexArray(sf::Triangles, this->triangles.size() * 3);
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		if (this->triangles[i] == nullptr || this->colors[i] == nullptr) {
			continue;
		}

		vertexArray[i * 3 + 0].position = this->triangles[i]->v1.getProjection(
			this->projectionMatrix,
			this->width,
			this->height
		);
		vertexArray[i * 3 + 0].color = *this->colors[i * 3 + 0];

		vertexArray[i * 3 + 1].position = this->triangles[i]->v2.getProjection(
			this->projectionMatrix,
			this->width,
			this->height
		);
		vertexArray[i * 3 + 1].color = *this->colors[i * 3 + 1];

		vertexArray[i * 3 + 2].position = this->triangles[i]->v3.getProjection(
			this->projectionMatrix,
			this->width,
			this->height
		);
		vertexArray[i * 3 + 2].color = *this->colors[i * 3 + 2];
	}
	target.draw(vertexArray, states);
}