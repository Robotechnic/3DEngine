#include "vector3.h"

template <typename T>
Vector3<T>::Vector3() : x(0), y(0), z(0) {}

template <typename T>
Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

template <typename T>
template <typename U>
Vector3<T>::Vector3(const Vector3<U>& other): x(other.x), y(other.y), z(other.z) {}

template <typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator/=(const Vector3<T>& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

template <typename T>
float Vector3<T>::dot(const Vector3<T>& other) {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

template <typename T>
Vector3<T> Vector3<T>::cross(const Vector3<T>& other) {
	float x = this->y * other.z - this->z * other.y;
	float y = this->z * other.x - this->x * other.z;
	float z = this->x * other.y - this->y * other.x;
	return Vector3<T>(x,y,z);
}

template <typename T>
float Vector3<T>::sqareLength() {
	return dot(*this);
}

template <typename T>
float Vector3<T>::length() {
	return sqrt(this->sqareLength());
}

template <typename T>
void Vector3<T>::normalize() {
	float length = this->length();
	x /= length;
	y /= length;
	z /= length;
}

template <typename T>
Vector3<T> Vector3<T>::normalized() {
	float length = this->length();
	return Vector3(x / length, y / length, z / length);
}


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
	return Vector3<T>(- right.x, - right.y, - right.z);
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