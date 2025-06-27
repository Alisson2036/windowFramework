#pragma once
#pragma comment(lib, "gdiplus.lib")
#include <iostream>
#include <Windows.h>
#include <gdiplus.h>
#include <algorithm>
#include <memory>
#include "..\Core\exception.h"
#include "..\Math\color.h"
#include "..\Math\vec2.h"


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

	// Classe para armazenar uma fonte
	class font
	{
	public:
		font(std::wstring fontName, float fontSize);

	private:
		friend Image;
		std::unique_ptr<Gdiplus::Font> fontLoaded;
	};

	static void initialize();
	static void uninitialize();

	~Image();

	Image() = default;
	Image(std::wstring fileName);
	Image(Image&) = delete;

	// Inicializa a imagem a partir de um arquivo
	void loadFile(std::wstring fileName);

	//inicializa a imagem a partir de um texto renderizado
	void fromRenderText(std::wstring text, font& textFont, int texSizeX, int texSizeY, color textColor);

	//inicializa a imagem com uma imagem em branco
	void fromBlank(int sizeX, int sizeY);


	// Define todos os pixels como transparentes
	void clear();
	void fill(color c);

	void drawPixel(unsigned int x, unsigned int y, color color);
	void drawLine(vec2 startPos, vec2 endPos, color c);
	void drawRectangle(vec2 pos, vec2 size, color c);
	void drawText(std::wstring text, font& textFont, vec2 position, color textColor);


	vec2 getResolution();
	data& getData();



private:
	Gdiplus::Bitmap* img = nullptr;
	data imageData = {};
	bool needsBufferUpdate = false;


};