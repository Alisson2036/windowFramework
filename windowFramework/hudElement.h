#pragma once
#include "object.h"
#include "pipeline.h"


class hudElement
{
public:
	void create(Texture& tex);


	void setPosition(vec2 pos);

	void draw(Pipeline& pipeline);

	void changeTexture(Texture& tex);
	

private:
	object obj;
	shader shader2d;
	vec2 resolution;

};