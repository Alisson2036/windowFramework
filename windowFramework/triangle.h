#pragma once
#include <DirectXMath.h>
#include "v.h"
#include "object.h"

#include "vertexShader.h"
#include "pixelShader.h"
#include "vertexBuffer.h"
#include "inputLayout.h"
#include "indexBuffer.h"
#include "vertexConstantBuffer.h"


#include "timer.h"


class Triangle : public Object
{
public:

	void create(vertex2d vertices[], int vertexCount, short indexes[], int indexCount)
	{

		vs.setDevice(device.Get());
		vs.setContext(context.Get());
		il.setDevice(device.Get());
		il.setContext(context.Get());
		ps.setDevice(device.Get());
		ps.setContext(context.Get());
		vb.setDevice(device.Get());
		vb.setContext(context.Get());
		ib.setDevice(device.Get());
		ib.setContext(context.Get());
		cvb.setDevice(device.Get());
		cvb.setContext(context.Get());


		//CRIA PIXEL SHADER
		vs.create(L"VertexShader.cso");

		//CRIA PIXEL SHADER
		ps.create(L"PixelShader.cso");

		//CRIA INPUT LAYOUT
		il.create(vs,
			{
				{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			},
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);


		//CRIA VERTEX BUFFER
		vb.create(
			vertices,
			vertexCount,
			sizeof(vertex2d)
		);

		//CRIA INDEX BUFFER
		ib.create(
			indexes,
			indexCount
		);

		//CRIA CONSTANT BUFFER
		DirectX::XMMATRIX b[] = {
			//DirectX::XMMatrixScaling(3.0f/4.0f,1.0f,1.0f) 
			DirectX::XMMatrixScaling(1.0f,1.0f,1.0f)
		};


		cvb.create(
			b,
			1,
			sizeof(DirectX::XMMATRIX)
		);

		//adiciona todos os bindables
		addBindable(&cvb);
		addBindable(&ps);
		addBindable(&vs);
		addBindable(&vb);
		addBindable(&il);
		addBindable(&ib);

		timeSinceCreation.reset();

		indicesNum = indexCount;
		isIndexedBool = true;
	}

	void update(float pos[3], float angle[3])
	{
		//float angle = timeSinceCreation.getPassedSeconds();

		DirectX::XMMATRIX finalMatrix = 
			DirectX::XMMatrixRotationX(angle[0]) * 
			DirectX::XMMatrixRotationY(angle[1]) * 
			DirectX::XMMatrixRotationZ(angle[2]);

		finalMatrix *= DirectX::XMMatrixTranslation(pos[0], pos[1], pos[2] + 4.0f);
		finalMatrix *= DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f);

		DirectX::XMMATRIX b[] = {
			//DirectX::XMMatrixScaling(3.0f/4.0f,1.0f,1.0f) 
			DirectX::XMMatrixTranspose(finalMatrix)
		};

		cvb.update(
			b
		);
		/*
		cvb.create(
			b,
			1,
			sizeof(DirectX::XMMATRIX)
		);
		*/
	}
	void draw() override
	{
	}

private:
	Timer timeSinceCreation;

	VertexShader vs;
	PixelShader ps;
	InputLayout il;
	VertexBuffer vb;
	ConstantVertexBuffer cvb;
	IndexBuffer ib;

};
