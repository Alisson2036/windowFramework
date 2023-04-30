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
}

Image::Image(std::wstring fileName)
{
	loadFile(fileName);
}

void Image::loadFile(std::wstring fileName)
{
	//criando bitmap no heap
	Gdiplus::Bitmap img(fileName.c_str());
	if (img.GetLastStatus())
		_throwMsg("Image does not exist");

	//configurando imageData
	imageData.pixelCount = img.GetWidth() * img.GetHeight();
	imageData.width = img.GetWidth();
	imageData.height = img.GetHeight();

	//alocando espaço na memoria para armazenar a imagem
	imageData.data = new pixel[imageData.pixelCount];

	//guardando os pixeis
	for (int i = 0; i < imageData.pixelCount; i++)
	{
		Gdiplus::Color c;
		img.GetPixel(
			i % imageData.width,
			int(i / imageData.height),
			reinterpret_cast<Gdiplus::Color*>(&imageData.data[i])
		);
	}
	
}

void Image::fromRenderText(std::wstring text, std::wstring font)
{
	Gdiplus::Bitmap* img = new Gdiplus::Bitmap(100,100);

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);

	Gdiplus::FontFamily fontFamily(L"Times New Roman");


	gfx->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixel);

	gfx->DrawString(
		text.c_str(),
		-1,
		new Gdiplus::Font(&fontFamily, 22, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
		Gdiplus::PointF(0, 0),
		new Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 255))
	);

	//colocando a imagem no buffer
	// 
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

Image::data& Image::getData()
{
	return imageData;//mudar para enviar por referencia depois
}
