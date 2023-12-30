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
	bool textureCoords = dataBuffer.containsType("TexCoord");

	if ( (!pos) && (!normals) && (!textureCoords) ) return;

	reserveVertexBuffer(obj.faces.size() * 3);
	int index = 0;

	for (objLoader::face& face : obj.faces)
	{
		for (int i = 0; i < 3; i++)
		{
			std::vector<float> temp;
			if (pos)
			{
				temp.push_back(obj.vertices[face.vertexIndex[i] - 1].x);
				temp.push_back(obj.vertices[face.vertexIndex[i] - 1].y);
				temp.push_back(obj.vertices[face.vertexIndex[i] - 1].z);
				dataBuffer.set(temp.data(), index, "Position");
				temp.clear();
			}

			if (textureCoords)
			{
				temp.push_back(obj.texCoord[face.textureIndex[i] - 1].u);
				temp.push_back(obj.texCoord[face.textureIndex[i] - 1].v);
				dataBuffer.set(temp.data(), index, "TexCoord");
				temp.clear();
			}

			if (normals)
			{
				temp.push_back(obj.normals[face.normalIndex[i] - 1].x);
				temp.push_back(obj.normals[face.normalIndex[i] - 1].y);
				temp.push_back(obj.normals[face.normalIndex[i] - 1].z);
				dataBuffer.set(temp.data(), index, "Normals");
				temp.clear();
			}
			index++;
		}
	}
}

void object::loadFromVertexArray(std::vector<vec3> vertexArray)
{
	if (!initialized) _throwMsg("Class not initialized");
	if(!dataBuffer.containsType("Position")) return;

	reserveVertexBuffer(vertexArray.size());
	for (int i = 0; i < vertexArray.size(); i++)
		dataBuffer.set(&(vertexArray[i]), i, "Position");
}

void object::loadFromVertexArray(std::vector<vec2> vertexArray)
{
	if (!initialized) _throwMsg("Class not initialized");
	if (!dataBuffer.containsType("Position2d")) return;

	reserveVertexBuffer(vertexArray.size());
	for (int i = 0; i < vertexArray.size(); i++)
		dataBuffer.set(&(vertexArray[i]), i, "Position2d");
}

void object::loadFromColorArray(std::vector<color> ColorArray)
{
	if (!initialized) _throwMsg("Class not initialized");
	if (!dataBuffer.containsType("Color")) return;

	reserveVertexBuffer(ColorArray.size());
	for (int i = 0; i < ColorArray.size(); i++)
		dataBuffer.set(&(ColorArray[i]), i, "Color");
}

void object::setVertexIndices(std::vector<int>& vertexIndices)
{
	indexes = vertexIndices;
}

int object::getVertexCount()
{
	return dataBuffer.getElementCount();
}

void object::lock()
{
	if (!initialized) _throwMsg("Class not initialized");

	dataBuffer.createVertexBuffer(vb);

	if (indexes.size() > 0)
		ib.create(indexes.data(), indexes.size());
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

void object::reserveVertexBuffer(int vertexCount)
{
	if (!initialized) _throwMsg("Class not initialized");
	if (vertexCount < getVertexCount()) _throwMsg("Not enough vertex data");

	dataBuffer.reserve(vertexCount);

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
