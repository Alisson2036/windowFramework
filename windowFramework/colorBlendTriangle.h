#pragma once
#include <DirectXMath.h>
#include "v.h"
#include "object.h"
#include "Instance.h"

#include "vertexShader.h"
#include "pixelShader.h"
#include "vertexBuffer.h"
#include "inputLayout.h"
#include "indexBuffer.h"
#include "vertexConstantBuffer.h"



class colorBlendTriangle : public Object
{
public:

	void create(vertex2d vertices[], int vertexCount, short indexes[], int indexCount)
	{

		//adiciona todos os bindables
		addBindable(&cvb);
		addBindable(&ps);
		addBindable(&vs);
		addBindable(&vb);
		addBindable(&il);
		addBindable(&ib);

		fillBindables();

		//CRIA PIXEL SHADER
		vs.create(L"colorBlendVS.cso");

		//CRIA PIXEL SHADER
		ps.create(L"colorBlendPS.cso");

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
			DirectX::XMMatrixScaling(1.0f,1.0f,1.0f)
		};

		cvb.create(
			b,
			1,
			sizeof(DirectX::XMMATRIX)
		);

		indicesNum = indexCount;
		isIndexedBool = true;
	}

	void update(Instance inst)
	{
		//float angle = timeSinceCreation.getPassedSeconds();

		DirectX::XMMATRIX finalMatrix = inst.getMatrix();
		DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.0f);

		DirectX::XMMATRIX b[] = {
			//DirectX::XMMatrixScaling(3.0f/4.0f,1.0f,1.0f) 
			finalMatrix * projectionMatrix
		};

		cvb.update(
			b
		);

	}
private:
	VertexShader vs;
	PixelShader ps;
	InputLayout il;
	VertexBuffer vb;
	ConstantVertexBuffer cvb;
	IndexBuffer ib;

};
