#include "texturedTriangle.h"

void TexturedTriangle::create(Pipeline* pipe, vertex3d vertices[], int vertexCount, short indexes[], int indexCount, Texture* tex)
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

void TexturedTriangle::create(Pipeline* pipe, objLoader& obj, Texture* tex)
{

	std::vector<vertex3d> vertices;
	std::vector<short> indexes;

	for (objLoader::face& face : obj.faces)
	{
		for (int i = 0; i < 3; i++)
		{
			vertex3d v
			{
				obj.vertices[face.vertexIndex[i]-1].x,
				obj.vertices[face.vertexIndex[i]-1].y,
				obj.vertices[face.vertexIndex[i]-1].z,

				obj.texCoord[face.textureIndex[i]-1].u,
				obj.texCoord[face.textureIndex[i]-1].v,
			};
			vertices.emplace_back(v);
		}
	}

	for (int i = 0; i < vertices.size(); i++)
		indexes.emplace_back(i);


	create(pipe, vertices.data(), vertices.size(), indexes.data(), indexes.size(), tex);

}

void TexturedTriangle::update(Instance inst)
{
	//float angle = timeSinceCreation.getPassedSeconds();

	DirectX::XMMATRIX finalMatrix = inst.getMatrix();
	DirectX::XMMATRIX projectionMatrix = inst.getProjectionMatrix();

	DirectX::XMMATRIX b[] = {
		finalMatrix * projectionMatrix
	};

	cvb.update(b);

}

void TexturedTriangle::draw()
{
	pipeline->bind(&desc);
}
