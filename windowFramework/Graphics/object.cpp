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

void object::load(MeshAsset* mesh)
{
	mesh->getData(&dataBuffer);
}

void object::loadFromObj(objLoader& obj)
{
	if (!initialized) _throwMsg("Class not initialized");

	bool pos = dataBuffer.containsType("Position");
	bool normals = dataBuffer.containsType("Normals");
	bool textureCoords = dataBuffer.containsType("TexCoord");
	bool tangent = dataBuffer.containsType("Tangents");

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
				temp.push_back(obj.texCoord[face.textureIndex[i] - 1].x);
				temp.push_back(obj.texCoord[face.textureIndex[i] - 1].y);
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
			if (tangent)
			{
				//vec3* pos1 = reinterpret_cast<vec3*>(&face.vertexIndex[0]);
				vec3 v0 = obj.vertices[face.vertexIndex[0] - 1];
				vec3 v1 = obj.vertices[face.vertexIndex[1] - 1];
				vec3 v2 = obj.vertices[face.vertexIndex[2] - 1];

				vec2 uv0 = obj.texCoord[face.textureIndex[0] - 1];
				vec2 uv1 = obj.texCoord[face.textureIndex[1] - 1];
				vec2 uv2 = obj.texCoord[face.textureIndex[2] - 1];

				vec3 deltaPos1 = v1 - v0;
				vec3 deltaPos2 = v2 - v0;
				vec2 deltaUV1 = uv1 - uv0;
				vec2 deltaUV2 = uv2 - uv0;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				//vec3 tangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
				vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
				//vec3 tangent(1.f, 0.f, 0.f);



				temp.push_back(tangent.x);
				temp.push_back(tangent.y);
				temp.push_back(tangent.z);
				dataBuffer.set(temp.data(), index, "Tangents");
				temp.clear();
			}
			index++;
		}
	}
}

void object::loadFromVertexArray(const std::vector<vec3>& vertexArray)
{
	if (!initialized) _throwMsg("Class not initialized");
	if(!dataBuffer.containsType("Position")) return;

	reserveVertexBuffer(vertexArray.size());
	for (int i = 0; i < vertexArray.size(); i++)
		dataBuffer.set(&(vertexArray[i]), i, "Position");
}

void object::loadFromVertexArray(const std::vector<vec2>& vertexArray)
{
	if (!initialized) _throwMsg("Class not initialized");
	if (!dataBuffer.containsType("Position2d")) return;

	reserveVertexBuffer(vertexArray.size());
	for (int i = 0; i < vertexArray.size(); i++)
		dataBuffer.set(&(vertexArray[i]), i, "Position2d");
}

void object::loadFromColorArray(const std::vector<color>& ColorArray)
{
	if (!initialized) _throwMsg("Class not initialized");
	if (!dataBuffer.containsType("Color")) return;

	reserveVertexBuffer(ColorArray.size());
	for (int i = 0; i < ColorArray.size(); i++)
		dataBuffer.set(&(ColorArray[i]), i, "Color");
}

void object::loadFromTexCoordArray(const std::vector<vec2>& texCoordArray)
{
	if (!initialized) _throwMsg("Class not initialized");
	if (!dataBuffer.containsType("TexCoord")) return;

	reserveVertexBuffer(texCoordArray.size());
	for (int i = 0; i < texCoordArray.size(); i++)
		dataBuffer.set(&(texCoordArray[i]), i, "TexCoord");
}

void object::setVertexIndices(const std::vector<int>& vertexIndices)
{
	indexes = vertexIndices;
}

void object::setInstancesPos(const std::vector<vec3>& positions)
{
	if (!pShader->hasPerInstanceData())
		_throwMsg("Object does not support instance data");

	UINT size = positions.size();
	if (instanceCount == size && vbInstance.isInitialized())
	{
		vbInstance.update(positions.data());
	}
	else
	{
		instanceCount = size;
		vbInstance.create(positions.data(), size, sizeof(vec3), true);
		vbInstance.setSlot(1);
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

void object::setScale(vec3 Scale)
{
	pos.setScale(Scale);
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

ConstantVertexBuffer* object::getConstantVertexBuffer()
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
