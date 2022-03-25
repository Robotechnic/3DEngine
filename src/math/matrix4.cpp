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


Matrix4 Matrix4::identity() {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result(i, j) = (i == j) ? 1 : 0;
		}
	}
	return result;
}

Matrix4 Matrix4::translation(Vector3<float> translation) {
	Matrix4 matrix;
	matrix(0, 0) = 1;
	matrix(1, 1) = 1;
	matrix(2, 2) = 1;
	matrix(3, 3) = 1;
	matrix(0, 3) = translation.x;
	matrix(1, 3) = translation.y;
	matrix(2, 3) = translation.z;

	return matrix;
}

Matrix4 Matrix4::translation(float x, float y, float z) {
	return Matrix4::translation(Vector3<float>(x, y, z));
}

Matrix4 Matrix4::rotation(Vector3<float> rotation) {
	float c1 = cos(rotation.x);
	float s1 = sin(rotation.x);
	float c2 = cos(rotation.y);
	float s2 = sin(rotation.y);
	float c3 = cos(rotation.z);
	float s3 = sin(rotation.z);

	// compute the rotation matrix with euler angles
	
	Matrix4 MX, MY, MZ;
	MX(0, 0) = 1;
	MX(1, 1) = c1;
	MX(1, 2) = -s1;
	MX(2, 2) = c1;
	MX(2, 1) = s1;
	MX(3, 3) = 1;

	MY(0, 0) = c2;
	MY(0, 2) = s2;
	MY(1, 1) = 1;
	MY(2, 0) = -s2;
	MY(2, 2) = c2;
	MY(3, 3) = 1;

	MZ(0, 0) = c3;
	MZ(0, 1) = -s3;
	MZ(1, 1) = c3;
	MZ(1, 0) = s3;
	MZ(2, 2) = 1;
	MZ(3, 3) = 1;
	
	return MX * MY * MZ;
}

Matrix4 Matrix4::rotation(float anglex, float angley, float anglez) {
	return Matrix4::rotation(Vector3f(anglex, angley, anglez));
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

Vector3f operator*(const Matrix4& left, const Vector3f& right) {
	float w = 0;
	Vector3f result;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result(i) += left.at(i, j) * right.at(j);
		}
		result(i) += left.at(i, 3);
	}
	for (int i = 0; i < 3; i++) {
		w += left.at(3, i) * right.at(i);
	}
	w += left.at(3, 3);

	if (w != 0) {
		result /= w;
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