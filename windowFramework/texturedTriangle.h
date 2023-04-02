#pragma once
#include <DirectXMath.h>
#include "Instance.h"
#include "pipeline.h"
#include "texture.h"


class TexturedTriangle
{
public:

	struct vertex3d
	{
		float x;
		float y;
		float z;
		float xTex;
		float yTex;
	};

	void create(Pipeline* pipe, vertex3d vertices[], int vertexCount, short indexes[], int indexCount, Texture* tex)
	{
		pipeline = pipe;


		desc.type = Pipeline::ObjectType::Textured;
		desc.indexBuffer = &ib;
		desc.vertexBuffer = &vb;
		desc.constantVertexBuffer = &cvb;
		desc.indicesNum = indexCount;
		desc.texture = tex;


		//CRIA VERTEX BUFFER
		vb.create(
			vertices,
			vertexCount,
			sizeof(vertex3d)
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


	void update(Instance inst)
	{
		//float angle = timeSinceCreation.getPassedSeconds();

		DirectX::XMMATRIX finalMatrix = inst.getMatrix();
		DirectX::XMMATRIX projectionMatrix = inst.getProjectionMatrix();

		DirectX::XMMATRIX b[] = {
			finalMatrix * projectionMatrix
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

	Pipeline* pipeline;

};
