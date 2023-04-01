#pragma once
#pragma comment(lib, "gdiplus.lib")
#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include "exception.h"


class Texture
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

	~Texture();

	Texture() = default;
	Texture(std::wstring fileName);
	Texture(Texture&) = default;

	void loadFile(std::wstring fileName);

	data& getData();



private:
	Gdiplus::Bitmap* img = nullptr;
	data imageData;


};