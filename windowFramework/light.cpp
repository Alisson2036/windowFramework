#include "light.h"

Light::Light()
{

	//DirectX::XMVECTOR b[] = {
	//	DirectX::XMVECTOR{0.0f,0.0f,0.0f},
	//	DirectX::XMVECTOR{0.0f,0.0f,0.0f},
	//	DirectX::XMVECTOR{0.0f,0.0f,0.0f},
	//	DirectX::XMVECTOR{0.0f,0.0f,0.0f},
	//	DirectX::XMVECTOR{0.0f,0.0f,0.0f},
	//};

	bufferStruct b[] =
	{
		{
			DirectX::XMVECTOR{0.0f,0.0f,0.0f},
			DirectX::XMMatrixIdentity()
		}
	};
	buf.create(
		b,
		1,
		5*sizeof(DirectX::XMVECTOR)
	);
}

void Light::updatePos(vec3 Position)
{
	DirectX::XMMATRIX mat;
	if (cam)
		mat = cam->getProjectionMatrix();
	else
		mat = DirectX::XMMatrixIdentity();


	bufferStruct b[] =
	{
		{
			DirectX::XMVECTOR{Position.x, Position.y, Position.z},
			mat
		}
	};
	

	buf.update(b);
}

void Light::setLightCam(Camera* _cam)
{
	cam = _cam;
}

void Light::bind(int bufferSlot)
{
	buf.setSlot(bufferSlot);
	buf.bind();
}
