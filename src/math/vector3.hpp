#pragma once

#include <SFML/System/Vector2.hpp>
#include <ostream>
#include <cmath>
#include "matrix4.hpp"

template <typename T>
class Vector3 {
	class Matrix4;
	public:
		Vector3(): x(0), y(0), z(0) {};
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {};
		Vector3(const Vector3<T>& other) : x(other.x), y(other.y), z(other.z) {};

		Vector3<T>& operator+=(const Vector3<T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vector3<T>& operator-=(const Vector3<T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Vector3<T>& operator*=(const Vector3<T>& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Vector3<T>& operator/=(const Vector3<T>& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		float dot(const Vector3<T>& other) {
			return x * other.x + y * other.y + z * other.z;
		}
		Vector3<T> cross(const Vector3<T>& other) {
			return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		float sqareLength() {
			return this->dot(*this);
		}
		
		float length() {
			return sqrt(this->sqareLength());
		}

		void normalize() {
			float length = this->length();
			x /= length;
			y /= length;
			z /= length;
		}

		Vector3<T> normalized() {
			float length = this->length();
			return Vector3<T>(x / length, y / length, z / length);
		}

		sf::Vector2f getProjection(Matrix4 projectionMatrix) {
			sf::Vector2f result;
			result.x = projectionMatrix(0, 0) * x + projectionMatrix(1, 0) * y + projectionMatrix(2, 0) * z + projectionMatrix(3, 0);
			result.y = projectionMatrix(0, 1) * x + projectionMatrix(1, 1) * y + projectionMatrix(2, 1) * z + projectionMatrix(3, 1);
			// result.z = projectionMatrix(0, 2) * x + projectionMatrix(1, 2) * y + projectionMatrix(2, 2) * z + projectionMatrix(3, 2);
			float w = projectionMatrix(0, 3) * x + projectionMatrix(1, 3) * y + projectionMatrix(2, 3) * z + projectionMatrix(3, 3);

			if (w != 0) {
				result /= w;
			}
			return result;
		}

		T x, y, z;
};

template <typename T>
Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right) {
	return Vector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}
template <typename T>
Vector3<T> operator-(const Vector3<T>& left, const Vector3<T>& right) {
	return Vector3<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}
template <typename T>
Vector3<T> operator-(const Vector3<T>& right) {
	return Vector3<T>(-right.x, -right.y, -right.z);
}
template <typename T>
Vector3<T> operator*(const Vector3<T>& left, const Vector3<T>& right) {
	return Vector3<T>(left.x * right.x, left.y * right.y, left.z * right.z);
}
template <typename T>
Vector3<T> operator/(const Vector3<T>& left, const Vector3<T>& right) {
	return Vector3<T>(left.x / right.x, left.y / right.y, left.z / right.z);
}
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;