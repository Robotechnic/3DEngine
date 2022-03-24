#include "vector3.hpp"

template<typename T>
sf::Vector2f Vector3<T>::getProjection(Matrix4 projectionMatrix, float width, float height) {
	sf::Vector2f result;
	result.x = projectionMatrix(0, 0) * x + projectionMatrix(1, 0) * y + projectionMatrix(2, 0) * z + projectionMatrix(3, 0);
	result.y = projectionMatrix(0, 1) * x + projectionMatrix(1, 1) * y + projectionMatrix(2, 1) * z + projectionMatrix(3, 1);
	// result.z = projectionMatrix(0, 2) * x + projectionMatrix(1, 2) * y + projectionMatrix(2, 2) * z + projectionMatrix(3, 2);
	float w = projectionMatrix(0, 3) * x + projectionMatrix(1, 3) * y + projectionMatrix(2, 3) * z + projectionMatrix(3, 3);

	if (w != 0) {
		result /= w;
	}
	
	result.x += 1;
	result.y += 1;
	result.x *= width / 2;
	result.y *= height / 2;

	return result;
}

template class Vector3<float>;