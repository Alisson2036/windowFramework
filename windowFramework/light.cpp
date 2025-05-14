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

	DirectX::XMVECTOR b[] =
	{
		DirectX::XMVECTOR{0.0f,0.0f,0.0f,0.0f}
	};
	buf.create(
		b,
		1,
		sizeof(DirectX::XMVECTOR)
	);
	buf.setSlot(0);

	//cria light projection matrix
	DirectX::XMMATRIX mat[] = { DirectX::XMMatrixIdentity() };//camera->getProjectionMatrix() };
	matBuf.create(mat, 1, sizeof(DirectX::XMMATRIX));
	matBuf.setSlot(2);
	
}

void Light::updatePos(vec3 Position)
{
	DirectX::XMMATRIX mat;
	if (cam)
		mat = cam->getMatrix();
	else
		mat = DirectX::XMMatrixIdentity();


	DirectX::XMVECTOR b[] =
	{
		DirectX::XMVECTOR{Position.x, Position.y, Position.z,1.0f},
	};
	DirectX::XMMATRIX pMat[] = { mat };

	buf.update(b);
	matBuf.update(pMat);
}

void Light::setLightCam(Camera* _cam)
{
	cam = _cam;
}

void Light::bind(int bufferSlot)
{
	buf.setSlot(bufferSlot);
	buf.bind();

	//mudar depois
	matBuf.bind();
}
