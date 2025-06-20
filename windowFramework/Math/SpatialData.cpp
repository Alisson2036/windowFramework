#include "SpatialData.h"



SpatialData::SpatialData()
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

SpatialData::SpatialData(vec3 Position, vec3 Angle)
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


void SpatialData::set(vec3 Position, vec3 Angle)
{
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	angle.x = Angle.x;
	angle.y = Angle.y;
	angle.z = Angle.z;
}

void SpatialData::move(vec3 Position)
{

	position.x += Position.x;
	position.y += Position.y;
	position.z += Position.z;
}

void SpatialData::rotate(vec3 Angle)
{

	angle.x += Angle.x;
	angle.y += Angle.y;
	angle.z += Angle.z;
}

void SpatialData::setScale(vec3 Scale)
{
	scale = Scale;
}

DirectX::XMMATRIX SpatialData::getMatrix()
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

