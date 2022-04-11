#include "scene/scene.hpp"

Scene::Scene(unsigned width, unsigned height, float fov, float near, float far) :
	wireframe(false),
	normals(false),
	faces(true),
	zbuffer(false),
	normalLength(1.0f),
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

/**
 * @brief initialise all pixels buffers to default values
 * 
 */
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
		this->zBuffer[i] = 0;
	}
}

/**
 * @brief change the render size
 * 
 * @param width render widt
 * @param height render height
 */
void Scene::resize(unsigned width, unsigned height){
	this->width = width;
	this->height = height;
	this->initPixelsBuffers();
	this->computeProjectionMatrix();
}

/**
 * @brief set the field of view
 * 
 * @param fov the value to set
 */
void Scene::setFov(float fov) {
	this->fov = fov;
	this->computeProjectionMatrix();
}

/**
 * @brief set camera based on pos and lookat pos
 * 
 * @param position position of the camera
 * @param lookat point the camera is looking at
 * @param up the up vector that determine the camera orientation
 */
void Scene::setCamera(const Vector3f position, const Vector3f lookat, const Vector3f up) {
	this->cameraPosition = position;
	this->cameraLookAt = lookat;
	this->cameraUp = up;
	this->computeCameraLookAt();
}

/**
 * @brief set camera position based on fps like position
 * 
 * @param pos position of the camera
 * @param theta the horizontal angle of the camera (yaw)
 * @param phi the vertical angle of the camera (pitch)
 * @param up the up vector that determine the camera orientation
 */
void Scene::setCamera(const Vector3f pos, const float theta, const float phi, const Vector3f up) {
	this->cameraPosition = pos;
	this->cameraLookAt = pos + Vector3f(
		cos(theta) * sin(phi),
		sin(theta) * sin(phi),
		cos(phi)
	);
	this->computeCameraLookAt();
}

/**
 * @brief compute projection matrix based on the scene parameters
 * 
 */
void Scene::computeProjectionMatrix() {
	this->projectionMatrix = Matrix4::projectionMatrix(this->fov, this->width / this->height, this->near, this->far);
}


/**
 * @brief compute camera transformation matrix based on the camera parameters
 * 
 */
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

/**
 * @brief get the last saved world state matrix
 * 
 */
void Scene::popMatrix() {
	this->worldStateMatrix = this->transformations.top();
	this->transformations.pop();
}

/**
 * @brief save the current world state matrix
 * 
 */
void Scene::pushMatrix() {
	this->transformations.push(this->worldStateMatrix);
}

/**
 * @brief translate the world origin
 * 
 * @param translation translation vector
 */
void Scene::translate(Vector3f translation) {
	this->worldStateMatrix = this->worldStateMatrix * Matrix4::translation(translation);
}

/**
 * @brief translate the world origin
 * @see Scene::translate(Vector3f translation)
 * 
 * @param x translation on x axis
 * @param y translation on y axis
 * @param z translation on z axis
 */
void Scene::translate(float x, float y, float z) {
	this->translate(Vector3f(x, y, z));
}

/**
 * @brief rotate the world origin around each axis (x,y and z)
 * 
 * @param rotation vector that contains the rotation angles
 */
void Scene::rotate(Vector3f rotation) {
	this->worldStateMatrix = this->worldStateMatrix * Matrix4::rotation(rotation);
}

/**
 * @brief rotate the world origin around each axis (x,y and z)
 * @see Scene::rotate(Vector3f rotation)
 * 
 * @param x x rotation angle
 * @param y y rotation angle
 * @param z z rotation angle
 */
void Scene::rotate(float x, float y, float z) {
	this->rotate(Vector3f(x, y, z));
}

/**
 * @brief add a shape to the scene
 * 
 * @param shape any shape that inherits from Shape
 */
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

/**
 * @brief check if a triangle is visible or not based on camera position
 * 
 * @param triangle triangle to check
 * @return bool if the triangle is visible or not
 */
inline bool Scene::isVisible(const Triangle &triangle) const {
	return triangle.getNormal().dot(triangle.v1) < 0;
}

/**
 * @brief change camera coordinate to put camera in the center of the world
 * 
 * @param triangle triangle to moove
 */
void Scene::setTrianglePosFromCamera(Triangle &triangle) const {
	for (int i = 0; i < 3; i++) {
		triangle(i) = this->cameraLookAtMatrix * triangle.at(i);
	}
	triangle.calculateNormal();
}

/**
 * @brief project a 3d vector to 2d projection vector
 * 
 * @param vector the vector to project
 * @return sf::Vector2f the projected vector
 */
sf::Vector2f Scene::getProjection(Vector3f vector) const {
	vector = this->projectionMatrix * vector;
	vector.x += 1;
	vector.y += 1;
	vector.x *= this->width / 2;
	vector.y *= this->height / 2;

	return sf::Vector2f(vector.x, vector.y);
}

/**
 * @brief clip each triangles of the scene against a plane with equation ax + by + cz + d = 0
 * 
 * @param planeNormal the vector normal to the plane x, y and z of the vector represents a, b and c
 * @param planeD if the plae is an affine plane, this is the d else set it to 0
 */
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
	this->triangles.clear();
	this->colors.clear();
	this->triangles = renderTriangles;
	this->colors = renderColors;
}

/**
 * @brief clip a triangle against a plane
 *
 * @param triangle the triangle to clip
 * @param color the color of the triangle
 * @see Scene::clipAgainstPlane(const Vector3f &planeNormal, const float &planeD)
 * @param planeNormal the normal vector of the plane
 * @param planeD d coefficient of the plane equation
 * @param renderTriangles all triangles to clip
 * @param renderColors all colors of each triangles
 */
void Scene::clipTriangle(
	const Triangle &triangle, const sf::Color &color,
	const Vector3f &planeNormal, const float &planeD,
	std::vector<Triangle> &renderTriangles, std::vector<sf::Color> &renderColors
) const {
	int pointIndex, inside;
	std::tie(pointIndex, inside) = triangle.getDistancesToPlane(planeNormal, planeD);

	if (inside == 0) { // if the triangle is outside the plane
		return;
	}

	if (inside == 3) { // if the triangle is inside the plane
		renderTriangles.push_back(triangle);
		renderColors.push_back(color);
		return;
	}

	// if there is points inside and outside the plane
	for (int i = 0; i < inside; i++) {
		renderColors.push_back(color);
	}

	Vector3f leftPoint, rightPoint;
	std::tie(leftPoint, rightPoint) = triangle.getLeftRightIntersection(planeNormal, planeD, pointIndex);

	// clip triangle
	if (inside == 1) {
		renderTriangles.push_back(Triangle(triangle.at(pointIndex), leftPoint,  rightPoint, triangle.getNormal()));
		return;
	}

	renderTriangles.push_back(Triangle(leftPoint, triangle.at((pointIndex + 1) % 3), triangle.at((pointIndex + 2) % 3), triangle.getNormal()));
	renderTriangles.push_back(Triangle(leftPoint, triangle.at((pointIndex + 2) % 3), rightPoint, triangle.getNormal()));
}

/**
 * @brief calculate one of tree baricentric coordinates of a point in a triangle
 * 
 * @param p1 one point of the triangle
 * @param p2 point after p1 in clockwise order
 * @param p3 point to check
 * @return float the barycentric component of p1
 */
float Scene::edgeFunction(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3) const {
	return ((p3.x - p1.x) * (p2.y - p1.y) - (p3.y - p1.y) * (p2.x - p1.x));
}

/**
 * @brief calculate z-index of a pixel based on barrycentric coordinates
 * 
 * @param w1 first barrycentric coordinate
 * @param w2 second barrycentric coordinate
 * @param w3 third barrycentric coordinate
 * @param v1 first point of the triangle
 * @param v2 second point of the triangle
 * @param v3 third point of the triangle
 * @return float the z-index for the given pixel
 */
float Scene::computeZIndex(float w1, float w2, float w3, Vector3f v1, Vector3f v2, Vector3f v3) {
	float z =  1 / (w1 * v1.z + w2 * v2.z  + w3 * v3.z); // compute z-index
	if (z > this->maxZ) {
		this->maxZ = z;
	}
	if (z < this->minZ) {
		this->minZ = z;
	}
	return z;
}

/**
 * @brief draw a triangle to the screen
 * 
 * @param t the triangle to draw
 * @param color the triangle color
 */
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
	
	for (unsigned int x = minX >= 0 ? minX : 0; x <= maxX && x <= this->width; x++) {
		for (unsigned int y = minY >= 0 ? minY : 0; y <= maxY && y <= this->height; y++) {
			area = edgeFunction(p1, p2, p3);
			w1 = edgeFunction(p2, p3, sf::Vector2f(x, y));
			w2 = edgeFunction(p3, p1, sf::Vector2f(x, y));
			w3 = edgeFunction(p1, p2, sf::Vector2f(x, y));

			if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
				w1 /= area;
				w2 /= area;
				w3 /= area;
				float z = computeZIndex(w1, w2, w3, v1, v2, v3);
				if (z > this->zBuffer[y * this->height + x]) {
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

/**
 * @brief draw each triangle of the scene
 * 
 */
void Scene::drawFaces() {
	for (long unsigned int  i = 0; i < this->triangles.size(); i++) {
		this->rasterizeTriangle(this->triangles[i], this->colors[i]);
	}
	
	this->texture.update(this->pixels);
}

/**
 * @brief draw the z-buffer to the screen
 * 
 */
void Scene::drawZBuffer() {
	float zValue;
	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			zValue = this->zBuffer[y * this->height + x] * 255;
			this->pixels[y * this->height * 4 + x * 4 + 0] = zValue;
			this->pixels[y * this->height * 4 + x * 4 + 1] = zValue;
			this->pixels[y * this->height * 4 + x * 4 + 2] = zValue;
			this->pixels[y * this->height * 4 + x * 4 + 3] = 255;
		}
	}
	this->texture.update(this->pixels);
}

/**
 * @brief draw each triangles in wireframe mode 
 * 
 * @return sf::VertexArray the vertex array to draw in line mode
 */
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

/**
 * @brief draw normals of each triangles
 * 
 * @return sf::VertexArray the vertex array that contains the normals in line mode
 */
sf::VertexArray Scene::drawNormals() const {
	sf::VertexArray vertexArray(sf::Lines, this->triangles.size() * 2);
	for (long unsigned int i = 0; i < this->triangles.size(); i++) {		
		Vector3f center = this->triangles[i].getCenter();
		Vector3f normal = this->triangles[i].getNormal();

		vertexArray[i * 2].position = this->getProjection(center);
		vertexArray[i * 2].color = sf::Color::Red;

		vertexArray[i * 2 + 1].position = this->getProjection(center + normal * this->normalLength);
		vertexArray[i * 2 + 1].color = sf::Color::Red;
	}
	return vertexArray;
}

/**
 * @brief clear all buffers and triangles list
 * 
 */
void Scene::clear() {
	this->triangles.clear();
	this->colors.clear();
	this->minZ = std::numeric_limits<float>::max();
	this->maxZ = std::numeric_limits<float>::min();
	for (long unsigned int i = 0; i < this->width * this->height * 4; i++) {
		this->pixels[i] = 0;
	}
	for (long unsigned int i = 0; i < this->width * this->height; i++) {
		this->zBuffer[i] = 0;
	}
}

/**
 * @brief draw the scene to the screen
 * 
 * @param target the window to draw in
 */
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

/**
 * @brief return the minimum ad maximum z-buffer values
 * 
 * @return std::tuple<float, float> min and max z-buffer values
 */
std::tuple<float, float> Scene::getZbound() const {
	return std::make_tuple(this->minZ, this->maxZ);
}