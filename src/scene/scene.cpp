#include "scene.hpp"

Scene::Scene(unsigned width, unsigned height, float fov, float near, float far) :
	wireframe(false),
	normals(false),
	faces(true),
	zbuffer(false),
	width(width),
	height(height),
	fov(fov),
	near(near),
	far(far)
{
	this->computeProjectionMatrix();
	this->worldStateMatrix = Matrix4::identity();
	this->initPixelsBuffers();
}

Scene::~Scene() {
	delete[] this->pixels;
	delete[] this->zBuffer;
}

void Scene::initPixelsBuffers() {
	int pixelsCount = this->width * this->height;
	// init pixel buffer
	this->pixels = new sf::Uint8[pixelsCount * 4];
	for (int i = 0; i < pixelsCount * 4; i++) {
		this->pixels[i] = 255;
	}

	this->texture.create(this->width, this->height);
	this->texture.update(this->pixels);
	this->sprite.setTexture(this->texture);

	// initialize z-buffer
	this->zBuffer = new float[pixelsCount];
	for (int i = 0; i < pixelsCount; i++) {
		this->zBuffer[i] = std::numeric_limits<float>::infinity();
	}
}

void Scene::resize(unsigned width, unsigned height){
	this->width = width;
	this->height = height;
	this->initPixelsBuffers();
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

	if (triangles.size() != colors.size()) {
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

inline bool Scene::isVisible(const Triangle &triangle) const {
	return triangle.getNormal().dot(triangle.v1) < 0;
}

void Scene::setTrianglePosFromCamera(Triangle &triangle) const {
	for (int i = 0; i < 3; i++) {
		triangle(i) = this->cameraLookAtMatrix * triangle.at(i);
	}
	triangle.calculateNormal();
}

sf::Vector2f Scene::getProjection(Vector3f vector) const {
	vector = this->projectionMatrix * vector;
	vector.x += 1;
	vector.y += 1;
	vector.x *= this->width / 2;
	vector.y *= this->height / 2;

	return sf::Vector2f(vector.x, vector.y);
}

void Scene::clipAgainstPlane(const Vector3f &planeNormal, const float &planeD) {
	std::vector<Triangle> renderTriangles;
	std::vector<sf::Color> renderColors;
	for (long unsigned int i = 0; i < this->triangles.size(); i++) {
		if (isVisible(this->triangles[i])) {
			this->clipTriangle(
				this->triangles[i], this->colors[i],
				planeNormal, planeD,
				renderTriangles, renderColors
			);
		}
	}
	this->triangles = renderTriangles;
	this->colors = renderColors;
}

void Scene::clipTriangle(
	const Triangle &triangle, const sf::Color &color,
	const Vector3f &planeNormal, const float &planeD,
	std::vector<Triangle> &renderTriangles, std::vector<sf::Color> &renderColors
) const {
	float distances[3];
	int inside = 0;
	for (int i = 0; i < 3; i++) {
		distances[i] = triangle.at(i).planeDistance(planeNormal, planeD);
		if (distances[i] > 0) {
			inside++;
		}
	}

	if (inside == 0) { // if the triangle is outside the plane
		return;
	}
	if (inside == 3) { // if the triangle is inside the plane
		renderTriangles.push_back(triangle);
		renderColors.push_back(color);
		return;
	}

	// if there is points inside and outside the plane
	for (int i = 0; i < inside; i++)
		renderColors.push_back(color);

	std::pair<Vector3f, Vector3f> intersections;
	Vector3f leftPoint, rightPoint;

	int pointIndex = 0;
	while ((inside == 1 && distances[pointIndex] < 0) || (inside == 2 && distances[pointIndex] > 0)) {
		pointIndex ++;
	}
	
	intersections = triangle.getLeftRightIntersection(planeNormal, planeD, pointIndex);
	std::tie(leftPoint, rightPoint) = intersections;

	// clip triangle
	if (inside == 1) {
		renderTriangles.push_back(Triangle(triangle.at(pointIndex), leftPoint,  rightPoint));
		return;
	}

	renderTriangles.push_back(Triangle(leftPoint, triangle.at((pointIndex + 1) % 3), triangle.at((pointIndex + 2) % 3)));
	renderTriangles.push_back(Triangle(leftPoint, triangle.at((pointIndex + 2) % 3), rightPoint));
}


float Scene::edgeFunction(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3) const {
	return ((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));
}

float Scene::computeZIndex(float w1, float w2, float w3, Vector3f v1, Vector3f v2, Vector3f v3) {
	float z =  w1 / v1.z + w2 / v2.z  + w3 / v3.z;
	return (1 / z - 1 / near)/(1 / this->far - 1 / this->near);
}

void Scene::rasterizeTriangle(const Triangle &t, const sf::Color& color) {
	Vector3f v1 = t.v1;
	Vector3f v2 = t.v2;
	Vector3f v3 = t.v3;

	sf::Vector2f p1 = this->getProjection(v1);
	sf::Vector2f p2 = this->getProjection(v2);
	sf::Vector2f p3 = this->getProjection(v3);

	int minX = std::min(p1.x, std::min(p2.x, p3.x));
	int maxX = std::max(p1.x, std::max(p2.x, p3.x));
	int minY = std::min(p1.y, std::min(p2.y, p3.y));
	int maxY = std::max(p1.y, std::max(p2.y, p3.y));

	float w1, w2, w3, area;
	
	for (int x = minX >= 0 ? minX : 0; x <= maxX && x <= this->width; x++) {
		for (int y = minY >= 0 ? minY : 0; y <= maxY && y <= this->height; y++) {
			area = edgeFunction(p1, p2, p3);
			w1 = edgeFunction(p3, p2, sf::Vector2f(x, y));
			w2 = edgeFunction(p2, p1, sf::Vector2f(x, y));
			w3 = edgeFunction(p1, p3, sf::Vector2f(x, y));
			if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
				w1 /= area;
				w2 /= area;
				w3 /= area;
				float z = computeZIndex(w1, w2, w3, v1, v2, v3);
				if (z < this->zBuffer[y * this->height + x]) {
					this->zBuffer[y * height + x] = z;
					this->pixels[y * this->height * 4 + x * 4 + 0] = color.r;
					this->pixels[y * this->height * 4 + x * 4 + 1] = color.g;
					this->pixels[y * this->height * 4 + x * 4 + 2] = color.b;
					this->pixels[y * this->height * 4 + x * 4 + 3] = 255;
				}
			}
		}
	}
}

void Scene::drawFaces() {
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		this->rasterizeTriangle(this->triangles[i], this->colors[i]);
	}
	
	this->texture.update(this->pixels);
}

void Scene::drawZBuffer() {
	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			this->pixels[y * this->height * 4 + x * 4 + 0] = this->zBuffer[y * this->height + x];
			this->pixels[y * this->height * 4 + x * 4 + 1] = this->zBuffer[y * this->height + x];
			this->pixels[y * this->height * 4 + x * 4 + 2] = this->zBuffer[y * this->height + x];
			this->pixels[y * this->height * 4 + x * 4 + 3] = 255;
		}
	}
	this->texture.update(this->pixels);
}

sf::VertexArray Scene::drawWireframe() const {
	sf::VertexArray vertexArray(sf::Lines, this->triangles.size() * 6);
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
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
		Vector3f center = this->triangles[i].getCenter();
		Vector3f normal = this->triangles[i].getNormal();

		vertexArray[i * 2].position = this->getProjection(center);
		vertexArray[i * 2].color = sf::Color::Red;

		vertexArray[i * 2 + 1].position = this->getProjection(center + normal * 10.0f);
		vertexArray[i * 2 + 1].color = sf::Color::Red;
	}
	return vertexArray;
}

void Scene::clear() {
	this->triangles.clear();
	this->colors.clear();
	for (long unsigned int i = 0; i < this->width * this->height * 4; i++) {
		this->pixels[i] = 0;
	}
	for (long unsigned int i = 0; i < this->width * this->height; i++) {
		this->zBuffer[i] = std::numeric_limits<float>::max();
	}
}

void Scene::draw(sf::RenderTarget& target) {
	for (long unsigned int i = 0; i < this->triangles.size(); i++) {
		setTrianglePosFromCamera(this->triangles[i]);
	}
	clipAgainstPlane(Vector3f(0,0,1), -this->near);
	clipAgainstPlane(Vector3f(0,0,-1), this->far);

	if (this->zbuffer) {
		this->drawFaces();
		this->drawZBuffer();
		target.draw(this->sprite);
		return;
	}

	if (this->faces) {
		this->drawFaces();
		target.draw(this->sprite);
	}

	if (this->wireframe) {
		target.draw(this->drawWireframe());
	}

	if (this->normals) {
		target.draw(this->drawNormals());
	}
}