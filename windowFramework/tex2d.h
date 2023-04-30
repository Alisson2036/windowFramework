#pragma once
#pragma once
#include <DirectXMath.h>
#include "Instance.h"
#include "pipeline.h"
#include "texture.h"


class Tex2d
{
private:
	struct vertex2d
	{
		float x;
		float y;
		float texX;
		float texY;
	};
	struct vertexBufferLayout
	{
		float pos[2];
	};

public:
	struct vec2
	{
		float x;
		float y;
	};
	struct square
	{
		vec2 pos;
		vec2 size;
		vec2 upLeftTexCoord;
		vec2 downRightTexCoord;
	};

	void create(Pipeline* pipe, square objDesc,Texture* tex)
	{
		pipeline = pipe;

		desc.type = Pipeline::Texture2d;
		desc.indexBuffer = &ib;
		desc.vertexBuffer = &vb;
		desc.indicesNum = 6;
		desc.constantVertexBuffer = &cvb;
		desc.texture = { tex };

		//criando os vertices para o vertexBuffer
		vertex2d vertices[4] =
		{
			//vertice cima esquerda
			vertex2d(
				objDesc.pos.x,
				objDesc.pos.y,
				objDesc.upLeftTexCoord.x,
				objDesc.upLeftTexCoord.y
			),
			//vertice cima direita
			vertex2d(
				objDesc.pos.x + objDesc.size.x,
				objDesc.pos.y,
				objDesc.downRightTexCoord.x,
				objDesc.upLeftTexCoord.y
			),
			//vertice baixo esquerda
			vertex2d(
				objDesc.pos.x,
				objDesc.pos.y - objDesc.size.y,
				objDesc.upLeftTexCoord.x,
				objDesc.downRightTexCoord.y
			),
			//vertice baixo direita
			vertex2d(
				objDesc.pos.x + objDesc.size.x,
				objDesc.pos.y - objDesc.size.y,
				objDesc.downRightTexCoord.x,
				objDesc.downRightTexCoord.y
			),
		};

		//criando o array para o IndexBuffer
		short indexes[6] =
		{
			0,1,2,
			1,3,2
		};

		//CRIA VERTEX BUFFER
		vb.create(
			vertices,
			4,
			sizeof(vertex2d)
		);

		//CRIA CONSTANT VERTEX BUFFER
		float b[] = {
			0.0f,
			0.0f,
			0.0f,
			0.0f
		};

		cvb.create(
			b,
			ARRAYSIZE(b),
			sizeof(float)
		);


		//CRIA INDEX BUFFER
		ib.create(
			indexes,
			6
		);
	}

	void update(float posOffsetX, float posOffsetY, float texOffsetX, float texOffsetY)
	{
		float b[] = {
			posOffsetX,
			posOffsetY,
			texOffsetX,
			texOffsetY
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

	Pipeline* pipeline;

	Pipeline::ObjectDescriptor desc;

};