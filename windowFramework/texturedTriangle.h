#pragma once
#include <DirectXMath.h>
#include "Instance.h"
#include "pipeline.h"
#include "texture.h"
#include "objLoader.h"


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

	void create(Pipeline* pipe, vertex3d vertices[], int vertexCount, short indexes[], int indexCount, Texture* tex);
	void create(Pipeline* pipe, objLoader& obj, Texture* tex);


	void update(Instance inst);

	void draw();
private:
	VertexBuffer vb;
	ConstantVertexBuffer cvb;
	IndexBuffer ib;

	Pipeline::ObjectDescriptor desc;

	Pipeline* pipeline;

};
