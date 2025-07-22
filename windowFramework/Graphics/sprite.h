#pragma once
#include "object.h"
#include "..\Resources\image.h"
#include "pipeline.h"


class sprite
{
public:
	void create(Image& tex);
	void create(Image& img, vec2 position, vec2 size);
	void create(Texture* img, vec2 position, vec2 size);



	void draw(Pipeline& pipeline);

	void update(Image& tex);
	

private:
	Texture* tex;
	object obj;
	Shader shader2d;
	vec2 resolution;

};