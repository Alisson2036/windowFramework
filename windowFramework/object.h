#pragma once
#include <DirectXMath.h>
#include "shader.h"
#include "objLoader.h"
#include "vertexConstantBuffer.h"
#include "Position.h"
#include "texture.h"

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

	//getters
	int getVertexCount();
	void lock();

	//cuida da posicao
	void set(vec3 position, vec3 angle);
	void move(vec3 vector);
	void rotate(vec3 angle);

	//texturas
	void setTexture(Texture* text, int slot);

private:
	bool initialized = false;

	shader* pShader;
	inputBuffer dataBuffer;
	VertexBuffer vb;

	//posicao do objeto
	ConstantVertexBuffer* getVertexBuffer();
	Position3d pos;
	ConstantVertexBuffer cvb;
	bool needUpdate = false;

	//texturas
	std::map<int, Texture*> textures;



};