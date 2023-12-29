#pragma once
#include <DirectXMath.h>
#include "shader.h"
#include "objLoader.h"
#include "vertexConstantBuffer.h"

class object
{
	friend class Pipeline;
public:
	object(object&) = delete;

	object(shader& shader);

	//loaders
	void loadFromObj(objLoader& obj);

	//getters
	int getVertexCount();

	void lock();

private:
	shader* pShader;
	inputBuffer dataBuffer;
	VertexBuffer vb;


	ConstantVertexBuffer cvb;


};