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
}

Image::Image(std::wstring fileName)
{
	loadFile(fileName);
}

void Image::loadFile(std::wstring fileName)
{
	//criando bitmap no heap
	img = std::make_unique<Gdiplus::Bitmap>(fileName.c_str());
	if (img->GetLastStatus())
		_throwMsg("Image does not exist");

	needsBufferUpdate = true;
	
}

void Image::fromRenderText(std::wstring text, font& textFont, int texSizeX, int texSizeY, color textColor)
{

	img = std::make_unique<Gdiplus::Bitmap>(texSizeX, texSizeY);
	
	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img.get());
	
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
	img = std::make_unique<Gdiplus::Bitmap>(sizeX, sizeY);

	needsBufferUpdate = true;
}

void Image::clear()
{

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img.get());
	gfx->Clear(Gdiplus::Color(0u, 0u, 0u, 0u));
	needsBufferUpdate = true;

	delete gfx;
}

void Image::fill(color c)
{

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img.get());
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
	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img.get());
	Gdiplus::SolidBrush brush(*reinterpret_cast<Gdiplus::Color*>(&c));
	Gdiplus::Pen pen(&brush);
	gfx->DrawLine(&pen, startPos.x, startPos.y, endPos.x, endPos.y);
	needsBufferUpdate = true;

	delete gfx;
}

void Image::drawRectangle(vec2 pos, vec2 size, color c)
{
	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img.get());
	Gdiplus::SolidBrush brush(*reinterpret_cast<Gdiplus::Color*>(&c));
	gfx->FillRectangle(&brush, pos.x, pos.y, size.x, size.y);
	needsBufferUpdate = true;

	delete gfx;
}

void Image::drawText(std::wstring text, font& textFont, vec2 position, color textColor)
{

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img.get());

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

int Image::drawBoundedText(std::wstring text, font& textFont, vec2 position, int maxWidth, color textColor)
{

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img.get());

	//Gdiplus::FontFamily fontFamily(L"Times New Roman");

	Gdiplus::RectF rect(position.x, position.y, (float)maxWidth, 1000.0f);
	Gdiplus::PointF pos(position.x, position.y);

	gfx->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

	Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*reinterpret_cast<Gdiplus::Color*>(&textColor));

	// Desenha string
	Gdiplus::Status s = gfx->DrawString(
		text.c_str(),
		-1,
		textFont.fontLoaded.get(),
		rect,
		nullptr,
		brush
	);

	// Calcula o tamanho da string 
	Gdiplus::RectF boundingBox;
	gfx->MeasureString(
		text.c_str(),
		-1,
		textFont.fontLoaded.get(),
		rect,
		nullptr,
		&boundingBox
	);



	needsBufferUpdate = true;

	delete brush;
	delete gfx;

	return boundingBox.Height;
}

vec2 Image::getResolution()
{
	return vec2(img->GetWidth(), img->GetHeight());
}

Image::data Image::getData()
{
	imageData.pixelCount = img->GetWidth() * img->GetHeight();
	imageData.width = img->GetWidth();
	imageData.height = img->GetHeight();

	// Retorna se o buffer nao precisar
	// de update
	if (!needsBufferUpdate)
	{
		imageData.data = buffer.get();
		return imageData;
	}

	// Alocando espaço na memoria para armazenar a imagem
	buffer = std::make_unique<color[]>(imageData.pixelCount);//new color[imageData.pixelCount];

	// Copiando os pixeis para o buffer
	Gdiplus::Rect imgRect(
		0,
		0,
		imageData.width,
		imageData.height
	);
	Gdiplus::PixelFormat pixelFormat = PixelFormat32bppARGB;
	std::unique_ptr<Gdiplus::BitmapData> bitmapData = std::make_unique<Gdiplus::BitmapData>();

	img->LockBits(
		&imgRect,
		Gdiplus::ImageLockModeRead,
		pixelFormat,
		bitmapData.get()
	);

	color* start = reinterpret_cast<color*>(bitmapData->Scan0);
	color* end = start + imageData.pixelCount;
	std::copy<color*, color*>(start, end, buffer.get());

	// Liberando memoria
	img->UnlockBits(bitmapData.get());

	// Adquirindo pointer
	imageData.data = buffer.get();

	// Salva que ja buffer foi atualizado
	needsBufferUpdate = false;

	return imageData;
}

Image::font::font(std::wstring fontName, float fontSize)
{
	Gdiplus::FontFamily fontFamily(fontName.c_str());
	fontLoaded = std::make_unique<Gdiplus::Font>( &fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
}
