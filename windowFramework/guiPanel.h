#pragma once
#include <vector>
#include <string>
#include "object.h"
#include "image.h"
#include "pipeline.h"


class guiPanel
{
public:
	guiPanel();

	void create(vec2 windowSize);

	void addValue(std::wstring name, float* value);

	void draw(Pipeline& pipeline);

private:
	struct info
	{
		std::wstring name;
		float* value;
	};

private:
	const float fontSize = 15.0f;

	Texture tex;
	object obj;
	shader shader2d;
	vec2 resolution;
	vec2 panelRes;
	Image gfx;
	Image::font font;

	std::vector<info> data;

};