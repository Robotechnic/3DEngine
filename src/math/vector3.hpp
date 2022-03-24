#pragma once

#include <SFML/System/Vector2.hpp>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include "matrix4.hpp"

class Matrix4;

template <typename T>
class Vector3 {
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

		T& operator()(const float coord) {
			if (coord == 0) {
				return x;
			} else if (coord == 1) {
				return y;
			} else if (coord == 2) {
				return z;
			} else {
				throw std::out_of_range("coordinate index out of range");
			}
		}

		T at(const float coord) const {
			if (coord == 0) {
				return x;
			} else if (coord == 1) {
				return y;
			} else if (coord == 2) {
				return z;
			} else {
				throw std::out_of_range("coordinate index out of range");
			}
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

		sf::Vector2f getProjection(Matrix4 projectionMatrix, float width, float height);

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