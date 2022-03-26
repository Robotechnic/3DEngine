#include "scene.hpp"

Scene::Scene(float width, float height, float fov, float near, float far) :
	wireframe(false),
	normals(false),
	faces(true),
	width(width),
	height(height),
	fov(fov),
	near(near),
	far(far)
{
	this->computeProjectionMatrix();
	this->worldStateMatrix = Matrix4::identity();
}

void Scene::resize(float width, float height){
	this->width = width;
	this->height = height;
	this->computeProjectionMatrix();
}

void Scene::setFov(float fov) {
	this->fov = fov;
	this->computeProjectionMatrix();
}

void Scene::setCamera(const Vector3f position, const Vector3f lookat, const Vector3f up) {
	this->cameraPosition = position;
	this->cameraLookAt = lookat;
	this->cameraUp = up;
	this->computeCameraLookAt();
}

void Scene::setCamera(const Vector3f pos, const float theta, const float phi, const Vector3f up) {
	this->cameraPosition = pos;
	this->cameraLookAt = pos + Vector3f(
		cos(theta) * sin(phi),
		sin(theta) * sin(phi),
		cos(phi)
	);
	this->computeCameraLookAt();
}

void Scene::computeProjectionMatrix() {
	this->projectionMatrix = Matrix4::projectionMatrix(this->fov, this->width / this->height, this->near, this->far);
}

void Scene::computeCameraLookAt(){
	Vector3f direction = this->cameraLookAt - this->cameraPosition;
	direction.normalize();
	Vector3f right = this->cameraUp.cross(direction);
	Vector3f up = direction.cross(right);
	cameraLookAtMatrix = Matrix4::identity();
	cameraLookAtMatrix.setLine(0, right);
	cameraLookAtMatrix.setLine(1, up);
	cameraLookAtMatrix.setLine(2, direction);

	Matrix4 posMatrix = Matrix4::identity();
	posMatrix.setColumn(3, -this->cameraPosition);

	this->cameraLookAtMatrix *= posMatrix;
}

void Scene::popMatrix() {
	this->worldStateMatrix = this->transformations.top();
	this->transformations.pop();
}

void Scene::pushMatrix() {
	this->transformations.push(this->worldStateMatrix);
}

void Scene::translate(Vector3f translation) {
	this->worldStateMatrix = this->worldStateMatrix * Matrix4::translation(translation);
}

void Scene::translate(float x, float y, float z) {
	this->translate(Vector3f(x, y, z));
}

void Scene::rotate(Vector3f rotation) {
	this->worldStateMatrix = this->worldStateMatrix * Matrix4::rotation(rotation);
}

void Scene::rotate(float x, float y, float z) {
	this->rotate(Vector3f(x, y, z));
}

void Scene::drawShape(Shape *shape) {
	std::vector <Triangle> triangles = shape->getTriangles();
	std::vector <sf::Color> colors = shape->getColors();

	if (triangles.size() * 3 != colors.size()) {
		throw std::runtime_error("triangles and colors size mismatch");
	}

	for (Triangle &t : triangles) {
		for (int i = 0; i < 3; i++) {
			t(i) = this->worldStateMatrix * t.at(i);
		}
		this->triangles.push_back(t);
	}

	this->colors.insert(this->colors.end(), colors.begin(), colors.end());
}

bool Scene::isVisible(const Triangle &triangle) const {
	float dot = triangle.normal.dot(triangle.v1 - this->cameraPosition);
	return dot < 0;
}

sf::Vector2f Scene::getProjection(Vector3f vector) const {
	vector = this->cameraLookAtMatrix * vector;
	vector = this->projectionMatrix * vector;

	vector.x += 1;
	vector.y += 1;
	vector.x *= this->width / 2;
	vector.y *= this->height / 2;

	return sf::Vector2f(vector.x, vector.y);
}

sf::VertexArray Scene::drawFaces() const {
	sf::VertexArray vertexArray(sf::Triangles, this->triangles.size() * 3);
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		if (!isVisible(this->triangles[i])) {
			continue;
		}
		for (int j = 0; j < 3; j++) {
			vertexArray[i * 3 + j].position = this->getProjection(this->triangles[i].at(j));
			vertexArray[i * 3 + j].color = this->colors[i];
		}
	}
	return vertexArray;
}

sf::VertexArray Scene::drawWireframe() const {
	sf::VertexArray vertexArray(sf::Lines, this->triangles.size() * 6);
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		if (!isVisible(this->triangles[i])) {
			continue;
		}
		for (int j = 0; j < 6; j+= 2) {
			vertexArray[i * 6 + j].position = this->getProjection(this->triangles[i].at(j % 3));
			vertexArray[i * 6 + j].color = this->colors[i];

			vertexArray[i * 6 + j + 1].position = this->getProjection(this->triangles[i].at((j + 1) % 3));
			vertexArray[i * 6 + j + 1].color = this->colors[i];
		}
	}
	return vertexArray;
}

sf::VertexArray Scene::drawNormals() const {
	sf::VertexArray vertexArray(sf::Lines, this->triangles.size() * 2);
	for (long unsigned int i = 0; i < this->triangles.size(); i++) {
		if (!isVisible(this->triangles[i])) {
			continue;
		}
		Vector3f center = this->triangles[i].getCenter();
		Vector3f normal = this->triangles[i].normal;

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