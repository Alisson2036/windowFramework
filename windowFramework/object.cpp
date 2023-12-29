#include "object.h"

object::object(shader& shader)
{
	create(shader);
}

void object::create(shader& shader)
{
	pShader = &shader;
	dataBuffer.create(shader.inputParams);

	//CRIA CONSTANT BUFFER
	DirectX::XMMATRIX b[] = {
		DirectX::XMMatrixScaling(1.0f,1.0f,1.0f)
	};

	cvb.create(
		b,
		1,
		sizeof(DirectX::XMMATRIX)
	);

	initialized = true;
}

void object::loadFromObj(objLoader& obj)
{
	if (!initialized) _throwMsg("Class not initialized");

	bool pos = dataBuffer.containsType("Position");
	bool normals = dataBuffer.containsType("Normals");
	bool texture = dataBuffer.containsType("TexCoord");

	if ( (!pos) && (!normals) && (!texture) ) return;

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
	if (!initialized) _throwMsg("Class not initialized");

	dataBuffer.createVertexBuffer(vb);
}

void object::set(vec3 position, vec3 angle)
{
	pos.set(position, angle);
	needUpdate = true;
}

void object::move(vec3 vector)
{
	pos.move(vector);
	needUpdate = true;
}

void object::rotate(vec3 angle)
{
	pos.rotate(angle);
	needUpdate = true;
}

void object::setTexture(Texture* text, int slot)
{
	textures[slot] = text;
}

ConstantVertexBuffer* object::getVertexBuffer()
{
	if (!initialized) _throwMsg("Class not initialized");

	//atualiza o constantBuffer, se necessario
	if (needUpdate)
	{
		DirectX::XMMATRIX finalMatrix = pos.getMatrix();

		DirectX::XMMATRIX b[] = {
			finalMatrix
		};

		cvb.update(b);
		needUpdate = false;
	}
	return &cvb;
}
