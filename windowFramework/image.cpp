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
		delete[] img;
	if (imageData.data)
		delete[] imageData.data;
}

Image::Image(std::wstring fileName)
{
	loadFile(fileName);
}

void Image::loadFile(std::wstring fileName)
{
	//apagando imgem, se exisitr
	if (img)
	{
		delete[] img;
		img = 0;
	}
	if (imageData.data)
	{
		delete[] imageData.data;
		imageData.data = 0;
	}

	//criando bitmap no heap
	img = new Gdiplus::Bitmap(fileName.c_str());
	if (img->GetLastStatus())
		_throwMsg("Image does not exist");

	needsBufferUpdate = true;
	
}

void Image::fromRenderText(std::wstring text, font& textFont, int texSizeX, int texSizeY, color textColor)
{
	//apagando imagem anterior, se existir
	if (img)
	{
		delete[] img;
		img = 0;
	}
	if (imageData.data)
	{
		delete[] imageData.data;
		imageData.data = 0;
	}

	
	img = new Gdiplus::Bitmap(texSizeX, texSizeY);
	
	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);
	
	//Gdiplus::FontFamily fontFamily(L"Times New Roman");

	Gdiplus::RectF rect(0.0f, 0.0f, (float)texSizeX, (float)texSizeY);
	

	gfx->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

	Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*reinterpret_cast<Gdiplus::Color*>(&textColor));
	
	gfx->DrawString(
		text.c_str(),
		-1,
		textFont.fontLoaded.get(),
		rect,
		nullptr,
		brush
	);


	
	needsBufferUpdate = true;
	
	delete brush;
	delete gfx;
}

void Image::fromBlank(int sizeX, int sizeY)
{
	//apagando imagem anterior, se existir
	if (img)
	{
		delete[] img;
		img = 0;
	}
	if (imageData.data)
	{
		delete[] imageData.data;
		imageData.data = 0;
	}

	img = new Gdiplus::Bitmap(sizeX, sizeY);

	needsBufferUpdate = true;
}

void Image::clear()
{

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);
	gfx->Clear(Gdiplus::Color(0u, 0u, 0u, 0u));
	needsBufferUpdate = true;

	delete gfx;
}

void Image::fill(color c)
{

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);
	gfx->Clear(Gdiplus::Color(c.a, c.r, c.g, c.b));
	needsBufferUpdate = true;

	delete gfx;
}

void Image::drawPixel(unsigned int x, unsigned int y, color color)
{
	img->SetPixel((INT)x, (INT)y, *reinterpret_cast<Gdiplus::Color*>(&color));
	needsBufferUpdate = true;
}

void Image::drawLine(vec2 startPos, vec2 endPos, color c)
{
	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);
	Gdiplus::SolidBrush brush(*reinterpret_cast<Gdiplus::Color*>(&c));
	Gdiplus::Pen pen(&brush);
	gfx->DrawLine(&pen, startPos.x, startPos.y, endPos.x, endPos.y);
	needsBufferUpdate = true;

	delete gfx;
}

void Image::drawRectangle(vec2 pos, vec2 size, color c)
{
	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);
	Gdiplus::SolidBrush brush(*reinterpret_cast<Gdiplus::Color*>(&c));
	gfx->FillRectangle(&brush, pos.x, pos.y, size.x, size.y);
	needsBufferUpdate = true;
}

void Image::drawText(std::wstring text, font& textFont, vec2 position, color textColor)
{

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);

	//Gdiplus::FontFamily fontFamily(L"Times New Roman");

	//Gdiplus::RectF rect(0.0f, 0.0f, (float)texSizeX, (float)texSizeY);
	Gdiplus::PointF pos(position.x, position.y);

	gfx->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

	Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*reinterpret_cast<Gdiplus::Color*>(&textColor));

	gfx->DrawString(
		text.c_str(),
		-1,
		textFont.fontLoaded.get(),
		pos,
		nullptr,
		brush
	);



	needsBufferUpdate = true;

	delete brush;
	delete gfx;
}

vec2 Image::getResolution()
{
	return vec2(img->GetWidth(), img->GetHeight());
}

Image::data& Image::getData()
{
	if(!needsBufferUpdate)
		return imageData;//mudar para enviar por referencia depois

	//colocando a imagem no buffer
	if (imageData.data)
	{
		delete[] imageData.data;
		imageData.data = 0;
	}
	//configurando imageData
	imageData.pixelCount = img->GetWidth() * img->GetHeight();
	imageData.width = img->GetWidth();
	imageData.height = img->GetHeight();

	//alocando espaço na memoria para armazenar a imagem
	imageData.data = new color[imageData.pixelCount];

	//guardando os pixeis
	Gdiplus::Rect imgRect(
		0,
		0,
		imageData.width,
		imageData.height
	);
	Gdiplus::PixelFormat pixelFormat = PixelFormat32bppARGB;
	Gdiplus::BitmapData* bitmapData = new Gdiplus::BitmapData;

	img->LockBits(
		&imgRect,
		Gdiplus::ImageLockModeRead,
		pixelFormat,
		bitmapData
	);

	color* start = reinterpret_cast<color*>(bitmapData->Scan0);
	color* end = start + imageData.pixelCount;
	std::copy<color*, color*>(start, end, imageData.data);

	//liberando memoria
	img->UnlockBits(bitmapData);
	delete bitmapData;

	//salva que ja buffer foi atualizado
	needsBufferUpdate = false;

	return imageData;
}

Image::font::font(std::wstring fontName, float fontSize)
{
	Gdiplus::FontFamily fontFamily(fontName.c_str());
	fontLoaded = std::make_unique<Gdiplus::Font>( &fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
}
