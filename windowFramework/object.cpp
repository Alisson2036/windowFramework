#include "object.h"

object::object(shader& shader)
	:
	pShader(&shader),
	dataBuffer(shader.inputParams)
{
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

void object::loadFromObj(objLoader& obj)
{
	for (objLoader::face& face : obj.faces)
	{
		for (int i = 0; i < 3; i++)
		{
			dataBuffer.push();

			std::vector<float> temp;

			temp.push_back(obj.vertices[face.vertexIndex[i] - 1].x);
			temp.push_back(obj.vertices[face.vertexIndex[i] - 1].y);
			temp.push_back(obj.vertices[face.vertexIndex[i] - 1].z);

			dataBuffer.setLast(temp.data(), "Position");

			//obj.texCoord[face.textureIndex[i] - 1].u;
			//obj.texCoord[face.textureIndex[i] - 1].v;
			//
			//obj.normals[face.normalIndex[i] - 1].x;
			//obj.normals[face.normalIndex[i] - 1].y;
			//obj.normals[face.normalIndex[i] - 1].z;

		}
	}
}

int object::getVertexCount()
{
	return dataBuffer.getElementCount();
}

void object::lock()
{
	dataBuffer.createVertexBuffer(vb);
}
