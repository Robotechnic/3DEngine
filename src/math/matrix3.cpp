#include "matrix3.hpp"

Matrix3::Matrix3() {
	for (int i = 0; i < 9; i++) {
		values[i] = 0;
	}
}
Matrix3::Matrix3(const Matrix3& other) {
	for (int i = 0; i < 9; i++) {
		values[i] = other.values[i];
	}
}
Matrix3::Matrix3(const float values[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->values[i * 3 + j] = values[i][j];
		}
	}
}

Matrix3 Matrix3::identitity() {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result(i, j) = (i == j) ? 1 : 0;
		}
	}
	return result;
}

float& Matrix3::operator()(unsigned line, unsigned column) {
	if (line > 2) {
		throw std::out_of_range("line index out of range");
	}
	if (column > 2) {
		throw std::out_of_range("column index out of range");
	}

	return this->values[line * 3 + column];
}

float Matrix3::at(unsigned line, unsigned column) const {
	if (line > 2) {
		throw std::out_of_range("line index out of range");
	}
	if (column > 2) {
		throw std::out_of_range("column index out of range");
	}

	return this->values[line * 3 + column];
}

Matrix3& Matrix3::operator+=(const Matrix3& other) {
	for (int i = 0; i < 9; i++) {
		this->values[i] += other.values[i];
	}
	return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& other) {
	for (int i = 0; i < 9; i++) {
		this->values[i] -= other.values[i];
	}
	return *this;
}

Matrix3& Matrix3::operator*=(const Matrix3& other) {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				result(i, j) += this->values[i * 3 + k] * other.values[k * 3 + j];
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		this->values[i] = result.values[i];
	}
	return *this;
}


Matrix3 operator+(const Matrix3& left, const Matrix3& right) {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result(i,j) = left.at(i, j) + right.at(i, j);
		}
	}
	return result;
}

Matrix3 operator-(const Matrix3& left, const Matrix3& right) {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result(i, j) = left.at(i, j) - right.at(i, j);
		}
	}
	return result;
}
Matrix3 operator*(const Matrix3& left, const Matrix3& right) {
	Matrix3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				result(i, j) += left.at(i, k) * right.at(k, j);
			}
		}
	}
	return result;
}

std::ostream& operator<<(std::ostream& stream, const Matrix3& matrix) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			stream << matrix.at(i, j) << " ";
		}
		stream << std::endl;
	}
	return stream;
}