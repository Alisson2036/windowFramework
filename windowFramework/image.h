#pragma once
#pragma comment(lib, "gdiplus.lib")
#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include "exception.h"


class Image
{
public:

	struct pixel
	{
		char r;
		char g;
		char b;
		char a;
	};
	struct data
	{
		pixel* data;
		int pixelCount;
		int width;
		int height;
	};

	static void initialize();
	static void uninitialize();

	~Image();

	Image() = default;
	Image(std::wstring fileName);
	Image(Image&) = default;

	void loadFile(std::wstring fileName);
	void fromRenderText(std::wstring text, std::wstring font);

	data& getData();



private:
	Gdiplus::Bitmap* img = nullptr;
	data imageData;


};