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

	void addValue(std::wstring name, float* value, bool readOnly = true);
	void addValue(std::wstring name, int* value, bool readOnly = true);

	void draw(Pipeline& pipeline);
	


private:
	enum class type
	{
		FLOAT,
		INTEGER,

	};
	struct panelValue
	{
		std::wstring name;
		void* pValue;
		type valueType;
		bool readOnly;
	};
	float drawElement(panelValue& val, float cursor);

private:
	const float fontSize = 15.0f;

	Texture tex;
	object obj;
	shader shader2d;
	vec2 resolution;
	vec2 panelRes;
	Image gfx;
	Image::font font;

	std::vector<panelValue> data;

};