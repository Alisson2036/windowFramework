#pragma once
#include "object.h"
#include "image.h"
#include "pipeline.h"


class hudElement
{
public:
	void create(Image& tex);
	void create(Image& img, vec2 position, vec2 size);



	void draw(Pipeline& pipeline);

	void update(Image& tex);
	

private:
	Texture tex;
	object obj;
	shader shader2d;
	vec2 resolution;

};