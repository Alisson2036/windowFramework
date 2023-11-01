#include "image.h"


ULONG_PTR token;


void Image::initialize()
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

void Image::uninitialize()
{
	if (token)
		Gdiplus::GdiplusShutdown(token);
}



Image::~Image()
{
	if (img)
		delete img;
	if (imageData.data)
		delete imageData.data;
}

Image::Image(std::wstring fileName)
{
	loadFile(fileName);
}

void Image::loadFile(std::wstring fileName)
{
	//apagando imgem, se exisitr
	if (img)
		delete img;
	if (imageData.data)
		delete imageData.data;

	//criando bitmap no heap
	Gdiplus::Bitmap img(fileName.c_str());
	if (img.GetLastStatus())
		_throwMsg("Image does not exist");

	//configurando imageData
	imageData.pixelCount = img.GetWidth() * img.GetHeight();
	imageData.width = img.GetWidth();
	imageData.height = img.GetHeight();

	//alocando espaço na memoria para armazenar a imagem
	imageData.data = new color[imageData.pixelCount];

	//guardando os pixeis
	for (int i = 0; i < imageData.pixelCount; i++)
	{
		Gdiplus::Color c;
		img.GetPixel(
			i % imageData.width,
			int(i / imageData.width),
			reinterpret_cast<Gdiplus::Color*>(&imageData.data[i])
		);
	}
	
}

void Image::fromRenderText(std::wstring text, std::wstring font, int texSizeX, int texSizeY, color textColor, float fontSize)
{
	//apagando imagem anterior, se existir
	if (img)
		delete img;
	if (imageData.data)
		delete imageData.data;


	img = new Gdiplus::Bitmap(texSizeX, texSizeY);

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);

	Gdiplus::FontFamily fontFamily(L"Times New Roman");

	Gdiplus::RectF rect(0.0f, 0.0f, (float)texSizeX, (float)texSizeY);
	

	gfx->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixel);

	gfx->DrawString(
		text.c_str(),
		-1,
		new Gdiplus::Font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
		rect,
		nullptr,
		new Gdiplus::SolidBrush(*reinterpret_cast<Gdiplus::Color*>(&textColor))//...funciona KASKSAKSAK
	);

	//colocando a imagem no buffer
	// 
	//configurando imageData
	imageData.pixelCount = img->GetWidth() * img->GetHeight();
	imageData.width = img->GetWidth();
	imageData.height = img->GetHeight();

	//alocando espaço na memoria para armazenar a imagem
	imageData.data = new color[imageData.pixelCount];

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

void Image::drawPixel(unsigned int x, unsigned int y, color color)
{
	if (x < (unsigned int)imageData.width && y < (unsigned int)imageData.height)
	{
		imageData.data[y * imageData.width + x] = color;
	}
}

Image::data& Image::getData()
{
	return imageData;//mudar para enviar por referencia depois
}
