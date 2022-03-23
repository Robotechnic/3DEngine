#pragma once

#include <cmath>
#include <stdexcept>
#include <ostream>
#include "vector3.hpp"

class Matrix4 {
	public:
		Matrix4();
		Matrix4(const Matrix4& other);
		Matrix4(float values[4][4]);

		float& operator()(unsigned line, unsigned column);
		float at(unsigned line, unsigned column) const;
		
		Matrix4& operator*=(const Matrix4& other);
		Matrix4& operator+=(const Matrix4& other);
		Matrix4& operator-=(const Matrix4& other);

		static Matrix4 projectionMatrix(float fov, float aspectRatio, float near, float far);
		static Matrix4 identitity();

	private:
		float values[16];
};

Matrix4 operator+(const Matrix4& left, const Matrix4& right);
Matrix4 operator-(const Matrix4& left, const Matrix4& right);
Matrix4 operator*(const Matrix4& left, const Matrix4& right);
std::ostream& operator<<(std::ostream& stream, const Matrix4& matrix);