#pragma once

#include <ostream>

template <typename T>
class Vector3 {
	public:
		Vector3<T>();
		Vector3<T>(T x, T y, T z);
		template <typename U>
		Vector3<T>(const Vector3<U>& other);

		Vector3<T>& operator+=(const Vector3<T>& other);
		Vector3<T>& operator-=(const Vector3<T>& other);
		Vector3<T>& operator*=(const Vector3<T>& other);
		Vector3<T>& operator/=(const Vector3<T>& other);

		float dot(const Vector3<T>& other);
		Vector3<T> cross(const Vector3<T>& other);

		float sqareLength();
		float length();

		void normalize();
		Vector3<T> normalized();

		T x, y, z;
};

template <typename T>
Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
Vector3<T> operator-(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
Vector3<T> operator-(const Vector3<T>& right);
template <typename T>
Vector3<T> operator*(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
Vector3<T> operator/(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec);

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;