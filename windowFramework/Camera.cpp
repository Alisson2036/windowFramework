#include "Camera.h"

Camera::Camera(float _position[3], float _angle[2])
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];

	angle[0] = _angle[0];
	angle[1] = _angle[1];
}

void Camera::setPositionAndAngle(float _position[3], float _angle[2])
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];

	angle[0] = _angle[0];
	angle[1] = _angle[1];
}

void Camera::move(float _position[3], float _angle[2])
{
	position[0] += _position[0];
	position[1] += _position[1];
	position[2] += _position[2];

	angle[0] += _angle[0];
	angle[1] += _angle[1];
}

void Camera::movePosition(float x, float y, float z)
{
	DirectX::XMVECTOR cameraDirection = DirectX::XMVectorSet(x, y, z, 1.0f);

	cameraDirection = DirectX::XMVector3Transform(
		cameraDirection,
		DirectX::XMMatrixTranspose(getRotationMatrix())
	);

	position[0] += DirectX::XMVectorGetX(cameraDirection);
	position[1] += DirectX::XMVectorGetY(cameraDirection);
	position[2] += DirectX::XMVectorGetZ(cameraDirection);
}

void Camera::moveAngle(float x, float y)
{
	angle[0] += x;
	angle[1] += y;
}

DirectX::XMMATRIX Camera::getMatrix()
{
	//DirectX::XMMatrixRotationNormal
	return DirectX::XMMATRIX(
		DirectX::XMMatrixTranslation(-position[0], -position[1], -position[2]) *
		getRotationMatrix() *
		getProjectionMatrix()
	);

}

DirectX::XMVECTOR Camera::getPositionVector()
{
	return DirectX::XMVECTOR({ position[0],position[1],position[2], 1.0f });
}

DirectX::XMMATRIX Camera::getProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.0f);
}

DirectX::XMMATRIX Camera::getRotationMatrix()
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixRotationY(angle[1]) *
		DirectX::XMMatrixRotationX(angle[0])
	);
}
