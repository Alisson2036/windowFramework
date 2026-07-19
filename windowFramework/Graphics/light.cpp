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
	buf.create(b);
	buf.setSlot(0);

	//cria light projection matrix
	DirectX::XMMATRIX mat[] = { DirectX::XMMatrixIdentity() };
	matBuf.create(mat);
	matBuf.setSlot(0);
	
}

void Light::updatePos(vec3 Position)
{
	if (cam)
	{
		//atualiza shadowmap projection matrix
		DirectX::XMMATRIX mat;
		mat = cam->getMatrix();
		DirectX::XMMATRIX pMat[] = { mat };
		matBuf.update(pMat);
	}


	DirectX::XMVECTOR b[] =
	{
		DirectX::XMVECTOR{Position.x, Position.y, Position.z,1.0f},
	};

	buf.update(b);
}

void Light::setShadowMapProjectionCam(Camera* _cam)
{
	cam = _cam;
}

Camera* Light::getCamera()
{
	return cam;
}

void Light::bind(int pixelBufferSlote, int vertexBufferSlot)
{
	buf.setSlot(pixelBufferSlote);
	buf.bind();

	//mudar depois
	if (cam)
	{
		matBuf.setSlot(vertexBufferSlot);
		matBuf.bind();
	}
}
