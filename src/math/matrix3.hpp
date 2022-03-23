#pragma once

#include <stdexcept>
#include <ostream>

#include "vector3.hpp"

class Matrix3 {
	public:
		Matrix3();
		Matrix3(const Matrix3& other);
		Matrix3(const float values[3][3]);

		float& operator()(unsigned line, unsigned column);
		float at(unsigned line, unsigned column) const;

		Matrix3& operator*=(const Matrix3& other);
		Matrix3& operator+=(const Matrix3& other);
		Matrix3& operator-=(const Matrix3& other);

		static Matrix3 identitity();


	private:
		float values[9];
};

Matrix3 operator+(const Matrix3& left, const Matrix3& right);
Matrix3 operator-(const Matrix3& left, const Matrix3& right);
Matrix3 operator*(const Matrix3& left, const Matrix3& right);

Vector3f operator*(const Matrix3& left, const Vector3f& right);

std::ostream& operator<<(std::ostream& stream, const Matrix3& matrix);