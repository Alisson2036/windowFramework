#pragma once
#include <DirectXMath.h>
#include "Position.h"
#include "pipeline.h"



class colorBlendTriangle
{
public:

	struct vertex2d
	{
		float x;
		float y;
		float z;
		char r;
		char g;
		char b;
		char a;
	};

	void create(Pipeline* pipe, vertex2d vertices[], int vertexCount, short indexes[], int indexCount)
	{
		pipeline = pipe;


		desc.type = Pipeline::ObjectType::ColorBlend;
		desc.indexBuffer = &ib;
		desc.vertexBuffer = &vb;
		desc.constantVertexBuffer = &cvb;
		desc.indicesNum = indexCount;


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

	}


	void update(Position3d inst)
	{
		//float angle = timeSinceCreation.getPassedSeconds();

		DirectX::XMMATRIX finalMatrix = inst.getMatrix();

		DirectX::XMMATRIX b[] = {
			finalMatrix
		};

		cvb.update(b);

	}

	void draw()
	{
		pipeline->bind(&desc);
	}
private:
	VertexBuffer vb;
	ConstantVertexBuffer cvb;
	IndexBuffer ib;

	Pipeline::ObjectDescriptor desc;

	Pipeline* pipeline = nullptr;

};
