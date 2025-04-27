#include "Camera.h"

Camera::Camera(vec3 Position, vec2 Angle)
{
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	angle.x = Angle.x;
	angle.y = Angle.y;
}

void Camera::setPositionAndAngle(vec3 Position, vec2 Angle)
{
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	angle.x = Angle.x;
	angle.y = Angle.y;
}

void Camera::move(vec3 Position, vec2 Angle)
{
	position.x += Position.x;
	position.y += Position.y;
	position.z += Position.z;

	angle.x += Angle.x;
	angle.y += Angle.y;
}

void Camera::movePosition(vec3 Position)
{
	DirectX::XMVECTOR cameraDirection = DirectX::XMVectorSet(Position.x, Position.y, Position.z, 1.0f);

	cameraDirection = DirectX::XMVector3Transform(
		cameraDirection,
		DirectX::XMMatrixTranspose(getRotationMatrix())
	);

	position.x += DirectX::XMVectorGetX(cameraDirection);
	position.y += DirectX::XMVectorGetY(cameraDirection);
	position.z += DirectX::XMVectorGetZ(cameraDirection);
}

void Camera::moveAngle(vec2 Angle)
{
	angle.x += Angle.x;
	angle.y += Angle.y;
}

void Camera::setScreenProportion(float proportion)
{
	screenProportion = proportion;
}

DirectX::XMMATRIX Camera::getMatrix()
{
	//DirectX::XMMatrixRotationNormal
	return DirectX::XMMATRIX(
		DirectX::XMMatrixTranslation(-position.x, -position.y, -position.z) *
		getRotationMatrix() *
		getProjectionMatrix()
	);

}

DirectX::XMVECTOR Camera::getPositionVector()
{
	return DirectX::XMVECTOR({ position.x,position.y,position.z, 1.0f });
}

DirectX::XMMATRIX Camera::getProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveLH(1.0f, screenProportion, 0.5f, 100.0f);
}

DirectX::XMMATRIX Camera::getRotationMatrix()
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixRotationY(angle.y) *
		DirectX::XMMatrixRotationX(angle.x)
	);
}
