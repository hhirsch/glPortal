/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix4f.cpp
** Implements a 4x4 matrix consisting of 16 float values and its helper functions
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#include "Matrix4f.hpp"

#include <engine/core/math/Math.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <math.h>
#include <sstream>

namespace glPortal {

/* Core */
Matrix4f::Matrix4f() {
	setIdentity();
}

void Matrix4f::setIdentity() {
	for (int i = 0; i < 16; i++) {
		if(i % 5 == 0) {
			a[i] = 1;
		} else {
			a[i] = 0;
		}
	}
}

void Matrix4f::translate(const Vector3f& v) {
	a[12] += a[0] * v.x + a[4] * v.y + a[8] * v.z;
	a[13] += a[1] * v.x + a[5] * v.y + a[9] * v.z;
	a[14] += a[2] * v.x + a[6] * v.y + a[10] * v.z;
	a[15] += a[3] * v.x + a[7] * v.y + a[11] * v.z;
}

void Matrix4f::rotate(float angle, float x, float y, float z) {
	float c = cos(Math::toRadians(angle));
	float s = sin(Math::toRadians(angle));
	float ic = 1 - c;

	float f0 = a[0] * ((x * x * ic) + c) + a[4] * ((x * y * ic) + (z * s)) + a[8] * ((x * z * ic) - (y * s));
	float f1 = a[1] * ((x * x * ic) + c) + a[5] * ((x * y * ic) + (z * s)) + a[9] * ((x * z * ic) - (y * s));
	float f2 = a[2] * ((x * x * ic) + c) + a[6] * ((x * y * ic) + (z * s)) + a[10] * ((x * z * ic) - (y * s));
	float f3 = a[3] * ((x * x * ic) + c) + a[7] * ((x * y * ic) + (z * s)) + a[11] * ((x * z * ic) - (y * s));

	float f4 = a[0] * ((x * y * ic) - (z * s)) + a[4] * ((y * y * ic) + c) + a[8] * ((y * z * ic) + (x * s));
	float f5 = a[1] * ((x * y * ic) - (z * s)) + a[5] * ((y * y * ic) + c) + a[9] * ((y * z * ic) + (x * s));
	float f6 = a[2] * ((x * y * ic) - (z * s)) + a[6] * ((y * y * ic) + c) + a[10] * ((y * z * ic) + (x * s));
	float f7 = a[3] * ((x * y * ic) - (z * s)) + a[7] * ((y * y * ic) + c) + a[11] * ((y * z * ic) + (x * s));

	float f8 = a[0] * ((x * z * ic) + (y * s)) + a[4] * ((y * z * ic) - (x * s)) + a[8] * ((z * z * ic) + c);
	float f9 = a[1] * ((x * z * ic) + (y * s)) + a[5] * ((y * z * ic) - (x * s)) + a[9] * ((z * z * ic) + c);
	float f10 = a[2] * ((x * z * ic) + (y * s)) + a[6] * ((y * z * ic) - (x * s)) + a[10] * ((z * z * ic) + c);
	float f11 = a[3] * ((x * z * ic) + (y * s)) + a[7] * ((y * z * ic) - (x * s)) + a[11] * ((z * z * ic) + c);

	a[0] = f0; a[1] = f1; a[2] = f2; a[3] = f3;
	a[4] = f4; a[5] = f5; a[6] = f6; a[7] = f7;
	a[8] = f8; a[9] = f9; a[10] = f10; a[11] = f11;
}

void Matrix4f::rotate(const Vector3f& euler) {
	rotate(euler.y, 0, 1, 0);
	rotate(euler.x, 1, 0, 0);
	rotate(euler.z, 0, 0, 1);
}

void Matrix4f::scale(const Vector3f& scale) {
	a[0] *= scale.x;
	a[1] *= scale.x;
	a[2] *= scale.x;
	a[3] *= scale.x;
	a[4] *= scale.y;
	a[5] *= scale.y;
	a[6] *= scale.y;
	a[7] *= scale.y;
	a[8] *= scale.z;
	a[9] *= scale.z;
	a[10] *= scale.z;
	a[11] *= scale.z;
}

Vector3f Matrix4f::transform(const Vector3f& v) {
	Vector3f dest;
	dest.x = a[0] * v.x + a[4] * v.y + a[8] * v.z;
	dest.y = a[1] * v.x + a[5] * v.y + a[9] * v.z;
	dest.z = a[2] * v.x + a[6] * v.y + a[10] * v.z;
	return dest;
}

float* Matrix4f::toArray() {
	return a;
}

/* Operator overloads */
bool Matrix4f::operator==(const Matrix4f& m) const {
	for (int i = 0; i < 16; i++) {
		if (a[i] != m.a[i]) {
			return false;
		}
	}
	return true;
}

bool Matrix4f::operator!=(const Matrix4f& m) const {
	for (int i = 0; i < 16; i++) {
		if (a[i] != m.a[i]) {
			return true;
		}
	}
	return false;
}

Matrix4f Matrix4f::operator*(const Matrix4f& m) const {
	Matrix4f dest;
	dest.a[0] = a[0] * m.a[0] + a[4] * m.a[1] + a[8] * m.a[2] + a[12] * m.a[3];
	dest.a[1] = a[1] * m.a[0] + a[5] * m.a[1] + a[9] * m.a[2] + a[13] * m.a[3];
	dest.a[2] = a[2] * m.a[0] + a[6] * m.a[1] + a[10] * m.a[2] + a[14] * m.a[3];
	dest.a[3] = a[3] * m.a[0] + a[7] * m.a[1] + a[11] * m.a[2] + a[15] * m.a[3];

	dest.a[4] = a[0] * m.a[4] + a[4] * m.a[5] + a[8] * m.a[6] + a[12] * m.a[7];
	dest.a[5] = a[1] * m.a[4] + a[5] * m.a[5] + a[9] * m.a[6] + a[13] * m.a[7];
	dest.a[6] = a[2] * m.a[4] + a[6] * m.a[5] + a[10] * m.a[6] + a[14] * m.a[7];
	dest.a[7] = a[3] * m.a[4] + a[7] * m.a[5] + a[11] * m.a[6] + a[15] * m.a[7];

	dest.a[8] = a[0] * m.a[8] + a[4] * m.a[9] + a[8] * m.a[10] + a[12] * m.a[11];
	dest.a[9] = a[1] * m.a[8] + a[5] * m.a[9] + a[9] * m.a[10] + a[13] * m.a[11];
	dest.a[10] = a[2] * m.a[8] + a[6] * m.a[9] + a[10] * m.a[10] + a[14] * m.a[11];
	dest.a[11] = a[3] * m.a[8] + a[7] * m.a[9] + a[11] * m.a[10] + a[15] * m.a[11];

	dest.a[12] = a[0] * m.a[12] + a[4] * m.a[13] + a[8] * m.a[14] + a[12] * m.a[15];
	dest.a[13] = a[1] * m.a[12] + a[5] * m.a[13] + a[9] * m.a[14] + a[13] * m.a[15];
	dest.a[14] = a[2] * m.a[12] + a[6] * m.a[13] + a[10] * m.a[14] + a[14] * m.a[15];
	dest.a[15] = a[3] * m.a[12] + a[7] * m.a[13] + a[11] * m.a[14] + a[15] * m.a[15];

	return dest;
}

/* Utility functions */
std::string str(const Matrix4f& m) {
	std::stringstream ss;
	ss << "[" << m.a[0] << ", " << m.a[4] << ", " << m.a[8] << ", " << m.a[12] << "]\n";
	ss << "[" << m.a[1] << ", " << m.a[5] << ", " << m.a[9] << ", " << m.a[13] << "]\n";
	ss << "[" << m.a[2] << ", " << m.a[6] << ", " << m.a[10] << ", " << m.a[14] << "]\n";
	ss << "[" << m.a[3] << ", " << m.a[7] << ", " << m.a[11] << ", " << m.a[15] << "]\n";

	return ss.str();
}

float determinant(const Matrix4f& m) {
  float f =
      m.a[0] * (m.a[5] * m.a[10] - m.a[6] * m.a[9])
      + m.a[1] * (m.a[6] * m.a[8] - m.a[4] * m.a[10])
      + m.a[2] * (m.a[4] * m.a[9] - m.a[5] * m.a[8]);
  return f;
}

Matrix4f inverse(const Matrix4f& m) {
  Matrix4f dest;
  float det = determinant(m);

  if (det != 0) {
     /* do it the ordinary way
      *
      * inv(A) = 1/det(A) * adj(T), where adj(T) = transpose(Conjugate Matrix)
      *
      * m00 m01 m02
      * m10 m11 m12
      * m20 m21 m22
      */
     float det_inv = 1.0f / det;

     // get the conjugate matrix
     float t00 = m.a[5] * m.a[10] - m.a[6] * m.a[9];
     float t01 = - m.a[4] * m.a[10] + m.a[6] * m.a[8];
     float t02 = m.a[4] * m.a[9] - m.a[5] * m.a[8];
     float t10 = - m.a[1] * m.a[10] + m.a[2] * m.a[9];
     float t11 = m.a[0] * m.a[10] - m.a[2] * m.a[8];
     float t12 = - m.a[0] * m.a[9] + m.a[1] * m.a[8];
     float t20 = m.a[1] * m.a[6] - m.a[2] * m.a[5];
     float t21 = -m.a[0] * m.a[6] + m.a[2] * m.a[4];
     float t22 = m.a[0] * m.a[5] - m.a[1] * m.a[4];

     dest.a[0] = t00 * det_inv;
     dest.a[5] = t11 * det_inv;
     dest.a[10] = t22 * det_inv;
     dest.a[1] = t10 * det_inv;
     dest.a[4] = t01 * det_inv;
     dest.a[8] = t02 * det_inv;
     dest.a[2] = t20 * det_inv;
     dest.a[6] = t21 * det_inv;
     dest.a[9] = t12 * det_inv;
  }
  return dest;
}

} /* namespace glPortal */
