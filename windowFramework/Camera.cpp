#include "Camera.h"

Camera::Camera(float _position[3], float _angle[3])
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];

	angle[0] = _angle[0];
	angle[1] = _angle[1];
	angle[2] = _angle[2];
}

void Camera::setPositionAndAngle(float _position[3], float _angle[3])
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];

	angle[0] = _angle[0];
	angle[1] = _angle[1];
	angle[2] = _angle[2];
}

void Camera::move(float _position[3], float _angle[3])
{
	position[0] += _position[0];
	position[1] += _position[1];
	position[2] += _position[2];

	angle[0] += _angle[0];
	angle[1] += _angle[1];
	angle[2] += _angle[2];
}

void Camera::movePosition(float x, float y, float z)
{
	position[0] += x;
	position[1] += y;
	position[2] += z;
}

void Camera::moveAngle(float x, float y, float z)
{
	angle[0] += x;
	angle[1] += y;
	angle[2] += z;
}

DirectX::XMMATRIX Camera::getMatrix()
{

	return DirectX::XMMATRIX(
		DirectX::XMMatrixRotationX(angle[0]) *
		DirectX::XMMatrixRotationY(angle[1]) *
		DirectX::XMMatrixRotationZ(angle[2]) *
		DirectX::XMMatrixTranslation(-position[0], -position[1], -position[2]));

}
