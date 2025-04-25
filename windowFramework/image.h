#pragma once
#pragma comment(lib, "gdiplus.lib")
#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include <algorithm>
#include "exception.h"
#include "color.h"
#include "vec2.h"


class Image
{
public:

	struct data
	{
		color* data;
		int pixelCount;
		int width;
		int height;
	};

	class font
	{
	public:
		font(std::wstring fontName, float fontSize);

	private:
		friend Image;
		Gdiplus::Font* fontLoaded;
	};

	static void initialize();
	static void uninitialize();

	~Image();

	Image() = default;
	Image(std::wstring fileName);
	Image(Image&) = default;

	void loadFile(std::wstring fileName);
	void fromRenderText(std::wstring text, font& textFont, int texSizeX, int texSizeY, color textColor);
	void fromBlank(int sizeX, int sizeY);

	void clear();

	void drawPixel(unsigned int x, unsigned int y, color color);
	void drawLine(vec2 startPos, vec2 endPos, color c);
	void drawRectangle(vec2 pos, vec2 size, color c);
	void drawText(std::wstring text, font& textFont, vec2 position, color textColor);

	data& getData();



private:
	Gdiplus::Bitmap* img = nullptr;
	data imageData;
	bool needsBufferUpdate = false;


};