#include "Instance.h"



Instance::Instance(vec3 Position, vec3 Angle)
{
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	angle.x = Angle.x;
	angle.y = Angle.y;
	angle.z = Angle.z;

}


void Instance::update(vec3 Position, vec3 Angle)
{
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	angle.x = Angle.x;
	angle.y = Angle.y;
	angle.z = Angle.z;
}

void Instance::move(vec3 Position, vec3 Angle)
{

	position.x += Position.x;
	position.y += Position.y;
	position.z += Position.z;

	angle.x += Angle.x;
	angle.y += Angle.y;
	angle.z += Angle.z;
}

DirectX::XMMATRIX Instance::getMatrix()
{

	return DirectX::XMMATRIX(
		DirectX::XMMatrixRotationX(angle.x) *
		DirectX::XMMatrixRotationY(angle.y) *
		DirectX::XMMatrixRotationZ(angle.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		//cam->getMatrix()
	);

}

