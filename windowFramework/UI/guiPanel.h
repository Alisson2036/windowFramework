#pragma once
#include <vector>
#include <string>
#include "..\Resources\image.h"
#include "..\Math\vec3.h"
#include "..\Graphics\pipeline.h"


class guiPanel
{
public:
	guiPanel();

	void create(vec2 windowSize);

	void addValue(std::wstring name, float* value, bool readOnly = true);
	void addValue(std::wstring name, int* value, bool readOnly = true);
	void addValue(std::wstring name, vec3* value, bool readOnly = true);

	// Gerencia a entrada do mouse e atualiza o estado do painel.
	// Retorna true se o mouse está sob um valor editável.
	bool handleInput(int mouseX, int mouseY, bool clicking);

	void draw(Pipeline& pipeline);
	


private:
	enum class type
	{
		NONE = 0,
		FLOAT,
		INTEGER,
	};
	struct panelValue
	{
		std::wstring name;
		void* pValue;
		type valueType;
		char arraySize = 1u;
		bool readOnly;
	};
	float drawElement(panelValue& val, float cursor, int pointerOffset);
	int changeValue(int mouseDiff, void* value, type valueType, int offset);

private:
	const float fontSize = 15.0f;

	int lastXMouse = 0;
	float lastCursor = 0.f;

	Texture tex;
	object obj;
	Shader shader2d;
	vec2 resolution;
	vec2 panelRes;
	Image gfx;
	Image::font font;

	std::vector<panelValue> data;

};