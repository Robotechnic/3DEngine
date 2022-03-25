#include "scene.hpp"

Scene::Scene(float width, float height, float fov, float near, float far) :
	wireframe(false),
	normals(false),
	faces(true),
	width(width),
	height(height)
{
	this->projectionMatrix = Matrix4::projectionMatrix(fov, width / height, near, far);
}

void Scene::setCamera(const Vector3f position, const Vector3f lookat) {
	this->cameraPosition = position;
	this->cameraLookat = lookat;
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


bool Scene::isVisible(const Triangle &triangle) const {
	float dot = triangle.normal.dot(triangle.v1 - this->cameraPosition);
	return dot < 0;
}

sf::Vector2f Scene::getProjection(Vector3f vector) const {
	vector -= this->cameraPosition;
	return vector.getProjection(this->projectionMatrix, this->width, this->height);
}

sf::VertexArray Scene::drawFaces() const {
	sf::VertexArray vertexArray(sf::Triangles, this->triangles.size() * 3);
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		if (this->triangles[i] == nullptr || this->colors[i] == nullptr || !isVisible(*this->triangles[i])) {
			continue;
		}
		for (int j = 0; j < 3; j++) {
			vertexArray[i * 3 + j].position = this->getProjection(this->triangles[i]->at(j));
			vertexArray[i * 3 + j].color = *this->colors[i];
		}
	}
	return vertexArray;
}

sf::VertexArray Scene::drawWireframe() const {
	sf::VertexArray vertexArray(sf::Lines, this->triangles.size() * 6);
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		if (this->triangles[i] == nullptr || this->colors[i] == nullptr || !isVisible(*this->triangles[i])) {
			continue;
		}
		for (int j = 0; j < 6; j+= 2) {
			vertexArray[i * 6 + j].position = this->getProjection(this->triangles[i]->at(j % 3));
			vertexArray[i * 6 + j].color = *this->colors[i];

			vertexArray[i * 6 + j + 1].position = this->getProjection(this->triangles[i]->at((j + 1) % 3));
			vertexArray[i * 6 + j + 1].color = *this->colors[i];
		}
	}
	return vertexArray;
}

sf::VertexArray Scene::drawNormals() const {
	sf::VertexArray vertexArray(sf::Lines, this->triangles.size() * 2);
	for (long unsigned int i = 0; i < this->triangles.size(); i++) {
		if (this->triangles[i] == nullptr || this->colors[i] == nullptr || !isVisible(*this->triangles[i])) {
			continue;
		}
		Vector3f center = this->triangles[i]->getCenter();
		Vector3f normal = this->triangles[i]->normal;

		vertexArray[i * 2].position = this->getProjection(center);
		vertexArray[i * 2].color = sf::Color::Red;

		vertexArray[i * 2 + 1].position = this->getProjection(center + normal * 10.0f);
		vertexArray[i * 2 + 1].color = sf::Color::Red;
	}
	return vertexArray;
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::VertexArray array;
	if (this->faces) {
		array = this->drawFaces();
	}

	if (this->wireframe) {
		target.draw(this->drawWireframe());
	} 
	
	target.draw(array, states);

	if (this->normals) {
		target.draw(this->drawNormals(), states);
	}
}