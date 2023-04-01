#include "texture.h"


ULONG_PTR token;


void Texture::initialize()
{
	if (!token)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = FALSE;

		Gdiplus::GdiplusStartup(&token, &input, nullptr);
	}
}

void Texture::uninitialize()
{
	if (token)
		Gdiplus::GdiplusShutdown(token);
}



Texture::~Texture()
{
	if (img)
		delete img;
}

Texture::Texture(std::wstring fileName)
{
	loadFile(fileName);
}

void Texture::loadFile(std::wstring fileName)
{
	//criando bitmap no heap
	img = new Gdiplus::Bitmap(fileName.c_str());


	//configurando imageData
	imageData.pixelCount = img->GetWidth() * img->GetHeight();
	imageData.width = img->GetWidth();
	imageData.height = img->GetHeight();

	//alocando espaço na memoria para armazenar a imagem
	imageData.data = new pixel[imageData.pixelCount];

	//guardando os pixeis
	for (int i = 0; i < imageData.pixelCount; i++)
	{
		Gdiplus::Color c;
		img->GetPixel(
			i % imageData.width,
			int(i / imageData.height),
			reinterpret_cast<Gdiplus::Color*>(&imageData.data[i])
		);
	}




}

Texture::data& Texture::getData()
{
	return &imageData;//mudar para enviar por referencia depois
}
