#pragma once
#include <DirectXMath.h>
#include "shader.h"
#include "objLoader.h"
#include "vertexConstantBuffer.h"
#include "Position.h"
#include "texture.h"
#include "vec3.h"
#include "vec2.h"
#include "color.h"

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
	void loadFromVertexArray(std::vector<vec3> vertexArray);
	void loadFromVertexArray(std::vector<vec2> vertexArray);
	void loadFromColorArray(std::vector<color> ColorArray);
	void setVertexIndices(std::vector<int>& vertexIndices);

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
	void reserveVertexBuffer(int vertexCount);

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

	//vertexIndex
	std::vector<int> indexes;



};