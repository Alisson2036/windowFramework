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
		addBindable(&vb);
		//addBindable(&il);
		addBindable(&ib);


		fillBindables();


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
	VertexBuffer vb;
	ConstantVertexBuffer cvb;
	IndexBuffer ib;

};
