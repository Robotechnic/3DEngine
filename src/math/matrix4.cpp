#include "matrix4.hpp"

Matrix4::Matrix4() {
	for (int i = 0; i < 16; i++) {
		this->values[i] = 0;
	}
}
Matrix4::Matrix4(const Matrix4& other) {
	for (int i = 0; i < 16; i++) {
		this->values[i] = other.values[i];
	}
}

Matrix4::Matrix4(float values[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->values[i * 4 + j] = values[i][j];
		}
	}
}

//get editable value at line and column
float& Matrix4::operator()(unsigned line, unsigned column) {
	if (line < 0 || line >3) {
		throw std::out_of_range("Line index out of range");
	}
	if (column < 0 || column > 3) {
		throw std::out_of_range("Column index out of range");
	}
	
	return this->values[line * 4 + column];
}

// get value read only at line and column
float Matrix4::at(unsigned line, unsigned column) const {
	if (line < 0 || line > 3) {
		throw std::out_of_range("Line index out of range");
	}
	if (column < 0 || column > 3) {
		throw std::out_of_range("Column index out of range");
	}

	return this->values[line * 4 + column];
}


Matrix4 Matrix4::projectionMatrix(float fov, float aspectRatio, float near, float far) {
	double S = 1 / tan(fov / 2 * M_PI / 180);
	Matrix4 matrix;
	matrix(0, 0) = aspectRatio*S;
	matrix(1, 1) = S;
	matrix(2, 2) = (far / (far - near));
	matrix(2, 3) = 1;
	matrix(3, 2) = -(far * near / (far - near));

	return matrix;
}


Matrix4 Matrix4::identitity() {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result(i, j) = (i == j) ? 1 : 0;
		}
	}
	return result;
}

Matrix4& Matrix4::operator+=(const Matrix4& other) {
	for (int i = 0; i < 16; i++) {
		this->values[i] += other.values[i];
	}
	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& other) {
	for (int i = 0; i < 16; i++) {
		this->values[i] -= other.values[i];
	}
	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& other) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result(i, j) += this->values[i * 4 + k] * other.values[k * 4 + j];
			}
		}
	}
	for (int i = 0; i < 16; i++) {
		this->values[i] = result.values[i];
	}
	return *this;
}


Matrix4 operator+(const Matrix4& left, const Matrix4& right) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result(i,j) = left.at(i, j) + right.at(i, j);
		}
	}
	return result;
}

Matrix4 operator-(const Matrix4& left, const Matrix4& right) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result(i, j) = left.at(i, j) - right.at(i, j);
		}
	}
	return result;
}
Matrix4 operator*(const Matrix4& left, const Matrix4& right) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result(i, j) += left.at(i, k) * right.at(k, j);
			}
		}
	}
	return result;
}
std::ostream& operator<<(std::ostream& stream, const Matrix4& matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			stream << matrix.at(i, j) << " ";
		}
		stream << std::endl;
	}
	return stream;
}