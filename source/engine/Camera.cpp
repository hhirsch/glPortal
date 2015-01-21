/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Camera.h
** Declares a camera class containing functions perform perspective or
** orthographic transformations on a matrix
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#include "Camera.hpp"

#include <engine/core/math/Matrix4f.hpp>
#include <engine/core/math/Math.hpp>
#include <math.h>

namespace glPortal {

Camera::Camera() {

}

Camera::Camera(float left, float right, float bottom, float top, float zNear, float zFar) :
	left(left), 
	right(right), 
	bottom(bottom),
	top(top),
	zNear(zNear),
	zFar(zFar) { }

Camera::Camera(float fovy, float aspect, float zNear, float zFar) :
	fovy(fovy),
	aspect(aspect),
	zNear(zNear),
	zFar(zFar) { }

void Camera::loadProjectionMatrix(Matrix4f& m) {
	m.setIdentity();
	if (perspective) {
		float fovyr = Math::toRadians(fovy);
		m.a[0] = (float)(1 / tan(fovyr / 2)) / aspect;
		m.a[5] = (float)(1 / tan(fovyr / 2));
		m.a[10] = (zNear + zFar) / (zNear - zFar);
		m.a[11] = -1;
		m.a[14] = (2 * zNear * zFar) / (zNear - zFar);
		m.a[15] = -0;
	}
	else {
		m.a[0] = 2 / (right - left);
		m.a[5] = 2 / (top - bottom);
		m.a[10] = -2 / (zFar - zNear);
		m.a[12] = (left - right) / (right - left);
		m.a[13] = (bottom - top) / (top - bottom);
		m.a[14] = (zNear - zFar) / (zFar - zNear);
	}
}

void Camera::setPerspective() {
	perspective = true;
}

void Camera::setOrthographic() {
	perspective = false;
}

void Camera::setFovy(float fovy) {
	this->fovy = fovy;
}

void Camera::setAspectRatio(float aspect) {
	this->aspect = aspect;
}

void Camera::setZNear(float zNear) {
	this->zNear = zNear;
}

void Camera::setZFar(float zFar) {
	this->zFar = zFar;
}

void Camera::setBounds(float left, float right, float bottom, float top) {
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
}

} /* namespace glPortal */
