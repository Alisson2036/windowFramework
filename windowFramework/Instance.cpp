#include "Instance.h"

static Camera* cam;

Instance::Instance(float _position[3], float _angle[3])
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];

	angle[0] = _angle[0];
	angle[1] = _angle[1];
	angle[2] = _angle[2];

}

void Instance::initializeCamera(Camera* _cam)
{
	cam = _cam;
}

void Instance::update(float _position[3], float _angle[3])
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];

	angle[0] = _angle[0];
	angle[1] = _angle[1];
	angle[2] = _angle[2];
}

void Instance::move(float _position[3], float _angle[3])
{

	position[0] += _position[0];
	position[1] += _position[1];
	position[2] += _position[2];

	angle[0] += _angle[0];
	angle[1] += _angle[1];
	angle[2] += _angle[2];
}

DirectX::XMMATRIX Instance::getMatrix()
{

	return DirectX::XMMATRIX(
		DirectX::XMMatrixRotationX(angle[0]) *
		DirectX::XMMatrixRotationY(angle[1]) *
		DirectX::XMMatrixRotationZ(angle[2]) *
		DirectX::XMMatrixTranslation(position[0], position[1], position[2]) *
		cam->getMatrix()
	);

}

DirectX::XMMATRIX Instance::getProjectionMatrix()
{
	return cam->getProjectionMatrix();
}
