#include "Position.h"



Position3d::Position3d()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	angle.x = 0.0f;
	angle.y = 0.0f;
	angle.z = 0.0f;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
}

Position3d::Position3d(vec3 Position, vec3 Angle)
{
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	angle.x = Angle.x;
	angle.y = Angle.y;
	angle.z = Angle.z;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
}


void Position3d::set(vec3 Position, vec3 Angle)
{
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	angle.x = Angle.x;
	angle.y = Angle.y;
	angle.z = Angle.z;
}

void Position3d::move(vec3 Position)
{

	position.x += Position.x;
	position.y += Position.y;
	position.z += Position.z;
}

void Position3d::rotate(vec3 Angle)
{

	angle.x += Angle.x;
	angle.y += Angle.y;
	angle.z += Angle.z;
}

void Position3d::setScale(vec3 Scale)
{
	scale = Scale;
}

DirectX::XMMATRIX Position3d::getMatrix()
{

	return DirectX::XMMATRIX(
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationX(angle.x) *
		DirectX::XMMatrixRotationY(angle.y) *
		DirectX::XMMatrixRotationZ(angle.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		//cam->getMatrix()
	);

}

