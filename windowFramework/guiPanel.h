#pragma once
#include "object.h"
#include "image.h"
#include "pipeline.h"


class guiPanel
{
public:
	void create(vec2 windowSize);



	void draw(Pipeline& pipeline);

private:
	Texture tex;
	object obj;
	shader shader2d;
	vec2 resolution;
	vec2 panelRes;
	Image gfx;

};