#pragma once
#include "object.h"
#include "image.h"
#include "pipeline.h"


class guiPanel
{
public:
	guiPanel();

	void create(vec2 windowSize);



	void draw(Pipeline& pipeline);

private:
	const float fontSize = 15.0f;

	Texture tex;
	object obj;
	shader shader2d;
	vec2 resolution;
	vec2 panelRes;
	Image gfx;
	Image::font font;

};