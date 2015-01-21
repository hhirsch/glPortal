/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix4f.hpp
** Declares a 4x4 matrix consisting of 16 float values and its helper functions
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#pragma once
#ifndef MATRIX4F_HPP
#define MATRIX4F_HPP

#include <string>

namespace glPortal {

class Vector3f;

class Matrix4f {

public:
	float a[16];

	/* Core */
	Matrix4f();
	void setIdentity();
	void translate(const Vector3f& v);
	void rotate(float angle, float x, float y, float z);
	void rotate(const Vector3f& euler);
	void scale(const Vector3f& scale);
	Vector3f transform(const Vector3f& v);

	float* toArray();

	/* Operator overloads */
	bool operator==(const Matrix4f& m) const;
	bool operator!=(const Matrix4f& m) const;
	Matrix4f operator*(const Matrix4f& m) const;

	/* Utility function */
	const std::string str();
};

float determinant(const Matrix4f& m);
Matrix4f inverse(const Matrix4f& m);

} /* namespace glPortal */

#endif /* MATRIX4F_HPP */
