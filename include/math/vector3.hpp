#pragma once

#include <SFML/System/Vector2.hpp>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <iostream>

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

		Vector3<T>& operator+=(const T& other) {
			x += other;
			y += other;
			z += other;
			return *this;
		}

		Vector3<T>& operator-=(const T& other) {
			x -= other;
			y -= other;
			z -= other;
			return *this;
		}

		Vector3<T>& operator*=(const T& other) {
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}

		Vector3<T>& operator/=(const T& other) {
			x /= other;
			y /= other;
			z /= other;
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

		float dot(const Vector3<T>& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		Vector3<T> cross(const Vector3<T>& other) {
			return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		float sqareLength() const {
			return this->dot(*this);
		}
		
		float length() const {
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

		T planeDistance(const Vector3<T>& planeNormal, const float& planeD) const {
			return T(
				planeNormal.dot(*this) + planeD
			);
		}

		static Vector3<T> segmentPlaneIntersection(const Vector3<T>& planeNormal, const float &planeD, const Vector3<T>& segmentStart, const Vector3<T>& segmentEnd) {
			if (planeNormal.dot(segmentEnd - segmentStart) == 0) {
				return Vector3<T>();
			}
			T t = ((- planeD - planeNormal.dot(segmentStart)) / planeNormal.dot(segmentEnd - segmentStart));
			return segmentStart + t * (segmentEnd - segmentStart);
		}

		T x, y, z;
};

template <typename T>
Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right) {
	return Vector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}
template <typename T>
Vector3<T> operator+(const Vector3<T>& left, const T& right) {
	return Vector3<T>(left.x + right, left.y + right, left.z + right);
}
template <typename T>
Vector3<T> operator+(const T& right, const Vector3<T>& left) {
	return Vector3<T>(left.x + right, left.y + right, left.z + right);
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
Vector3<T> operator-(const Vector3<T>& left, const T& right) {
	return Vector3<T>(left.x - right, left.y - right, left.z - right);
}
template <typename T>
Vector3<T> operator-(const T& right, const Vector3<T>& left) {
	return Vector3<T>(left.x - right, left.y - right, left.z - right);
}
template <typename T>
Vector3<T> operator*(const Vector3<T>& left, const Vector3<T>& right) {
	return Vector3<T>(left.x * right.x, left.y * right.y, left.z * right.z);
}
template <typename T>
Vector3<T> operator*(const Vector3<T>& left, const T& right) {
	return Vector3<T>(left.x * right, left.y * right, left.z * right);
}
template <typename T>
Vector3<T> operator*(const T& right, const Vector3<T>& left) {
	return Vector3<T>(left.x * right, left.y * right, left.z * right);
}
template <typename T>
Vector3<T> operator/(const Vector3<T>& left, const Vector3<T>& right) {
	return Vector3<T>(left.x / right.x, left.y / right.y, left.z / right.z);
}
template <typename T>
Vector3<T> operator/(const Vector3<T>& left, const T& right) {
	return Vector3<T>(left.x / right, left.y / right, left.z / right);
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

typedef Vector3<float>  Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int>    Vector3i;