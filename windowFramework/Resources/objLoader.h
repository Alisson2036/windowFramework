#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "..\Math\vec3.h"
#include "..\Math\vec2.h"
#include "..\Core\exception.h"


class objLoader
{
public:

	void fromFile(std::string fileName);
	int getVertexCount();
	/*
	struct vertex
	{
		float x;
		float y;
		float z;
	};
	struct vertexNormal
	{
		float x;
		float y;
		float z;
	};
	struct textureCoord
	{
		float u;
		float v;
	};*/
	struct face
	{
		int vertexIndex[3];
		int textureIndex[3];
		int normalIndex[3];
	};



	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> texCoord;
	std::vector<face> faces;

};