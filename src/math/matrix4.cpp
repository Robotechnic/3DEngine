#include "math/matrix4.hpp"

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

/**
 * @brief set the value of a line in the matrix with a vector
 * 
 * @param line the line to set
 * @param vector the values to put in
 */
void Matrix4::setLine(unsigned line, Vector3f vector) {
	if (line < 0 || line > 3) {
		throw std::out_of_range("Line index out of range");
	}

	this->values[line * 4] = vector.x;
	this->values[line * 4 + 1] = vector.y;
	this->values[line * 4 + 2] = vector.z;
}

/**
 * @brief set the value of a column in the matrix with a vector
 * 
 * @param column the column to set
 * @param vector the values to put in
 */
void Matrix4::setColumn(unsigned column, Vector3f vector) {
	if (column < 0 || column > 3) {
		throw std::out_of_range("Column index out of range");
	}

	this->values[column] = vector.x;
	this->values[column + 4] = vector.y;
	this->values[column + 8] = vector.z;
}

/**
 * @brief return a projection matrix based on the given parameters
 * 
 * @param fov the field of view of the projection
 * @param aspectRatio the aspect ration of the projection rectangle
 * @param near the minimum render distance
 * @param far the maximum render distance
 * @return Matrix4 the resulting projection matrix
 */
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

/**
 * @brief return an identity matrix of size 4x4
 * 
 * @return Matrix4 the identity matrix
 */
Matrix4 Matrix4::identity() {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result(i, j) = (i == j) ? 1 : 0;
		}
	}
	return result;
}

/**
 * @brief return a translation matrix
 * 
 * @param translation the translation to apply
 * @return Matrix4 the resulting translation matrix
 */
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

/**
 * @brief build a translation matrix
 * @see Matrix4::translation(Vector3<float> translation)
 * @return Matrix4 the resulting translation matrix
 */
Matrix4 Matrix4::translation(float x, float y, float z) {
	return Matrix4::translation(Vector3<float>(x, y, z));
}

/**
 * @brief return a rotation matrix
 * 
 * @param rotation the rotation to apply with each rotation around an axis (x, y and z)
 * @return Matrix4 the resulting rotation matrix
 */
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

/**
 * @brief build a rotation matrix
 * 
 * @see Matrix4::rotation(Vector3<float> rotation)
 * @return Matrix4 the resulting rotation matrix
 */
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