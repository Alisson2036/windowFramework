#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iostream>


class objLoader
{
public:

	void fromFile(std::string fileName);
	int getVertexCount();

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
	};
	struct face
	{
		int vertexIndex[3];
		int textureIndex[3];
		int normalIndex[3];
	};



	std::vector<vertex> vertices;
	std::vector<vertexNormal> normals;
	std::vector<textureCoord> texCoord;
	std::vector<face> faces;

};