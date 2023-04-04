#include "texturedTriangle.h"

void TexturedTriangle::create(Pipeline* pipe, vertex3d vertices[], int vertexCount, Texture* tex)
{
	pipeline = pipe;


	desc.type = Pipeline::ObjectType::Textured;
	desc.vertexBuffer = &vb;
	desc.constantVertexBuffer = &cvb;
	desc.indicesNum = vertexCount;
	desc.texture = tex;


	//CRIA VERTEX BUFFER
	vb.create(
		vertices,
		vertexCount,
		sizeof(vertex3d)
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

				obj.normals[face.normalIndex[i] - 1].x,
				obj.normals[face.normalIndex[i] - 1].y,
				obj.normals[face.normalIndex[i] - 1].z,

			};
			vertices.emplace_back(v);
		}
	}
	


	create(pipe, vertices.data(), vertices.size(), tex);

}

void TexturedTriangle::update(Instance inst)
{
	//float angle = timeSinceCreation.getPassedSeconds();

	DirectX::XMMATRIX finalMatrix = inst.getMatrix();

	DirectX::XMMATRIX b[] = {
		finalMatrix //* inst.getProjectionMatrix() 
	};

	cvb.update(b);

}

void TexturedTriangle::draw()
{
	pipeline->bind(&desc);
}
