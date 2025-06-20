#pragma once
#include <DirectXMath.h>
#include "..\Resources\objLoader.h"
#include "..\Math\SpatialData.h"
#include "..\Math\vec3.h"
#include "..\Math\vec2.h"
#include "..\Math\color.h"
#include "Shader\shader.h"
#include "Bindables\vertexConstantBuffer.h"
#include "Bindables\texture.h"
#include "Bindables\indexBuffer.h"

class object
{
	friend class Pipeline;
public:
	//impedir construtores
	object(object&) = delete;

	//construtores padroes
	object() = default;
	object(shader& shader);
	void create(shader& shader);

	//loaders
	void loadFromObj(objLoader& obj);
	void loadFromVertexArray(const std::vector<vec3>& vertexArray);
	void loadFromVertexArray(const std::vector<vec2>& vertexArray);
	void loadFromColorArray(const std::vector<color>& ColorArray);
	void loadFromTexCoordArray(const std::vector<vec2>& texCoordArray);
	void setVertexIndices(const std::vector<int>& vertexIndices);
	void setInstancesPos(const std::vector<vec3>& positions);

	//getters
	int getVertexCount();

	//setters
	void set(vec3 position, vec3 angle);
	void move(vec3 vector);
	void rotate(vec3 angle);
	void setScale(vec3 Scale);
	void setTexture(Texture* text, int slot);

	void lock();

private:
	ConstantVertexBuffer* getConstantVertexBuffer();
	void reserveVertexBuffer(int vertexCount);

private:

	bool initialized = false;

	shader* pShader;
	inputBuffer dataBuffer;
	IndexBuffer ib;
	VertexBuffer vb;
	VertexBuffer vbInstance;

	//posicao do objeto
	SpatialData pos;
	ConstantVertexBuffer cvb;
	bool needUpdate = false;

	//texturas
	std::map<int, Texture*> textures;

	//vertexIndex
	std::vector<int> indexes;
	//instance count 
	unsigned int instanceCount = 0u;



};