#pragma once

#include <cmath>
#include <stdexcept>
#include <ostream>
#include "math/vector3.hpp"

class Matrix4 {
	public:
		Matrix4();
		Matrix4(const Matrix4& other);
		Matrix4(float values[4][4]);

		float& operator()(unsigned line, unsigned column);
		float at(unsigned line, unsigned column) const;
		
		void setLine(unsigned line, Vector3f vector);
		void setColumn(unsigned column, Vector3f vector);

		Matrix4& operator*=(const Matrix4& other);
		Matrix4& operator+=(const Matrix4& other);
		Matrix4& operator-=(const Matrix4& other);

		static Matrix4 projectionMatrix(float fov, float aspectRatio, float near, float far);
		static Matrix4 identity();
		static Matrix4 translation(Vector3<float> translation);
		static Matrix4 translation(float x, float y, float z);
		static Matrix4 rotation(Vector3<float> rotation);
		static Matrix4 rotation(float anglex, float angley, float anglez);

	private:
		float values[16];
};

Matrix4 operator+(const Matrix4& left, const Matrix4& right);
Matrix4 operator-(const Matrix4& left, const Matrix4& right);
Matrix4 operator*(const Matrix4& left, const Matrix4& right);
Vector3f operator*(const Matrix4& left, const Vector3f& right);


std::ostream& operator<<(std::ostream& stream, const Matrix4& matrix);