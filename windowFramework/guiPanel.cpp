#include "guiPanel.h"

guiPanel::guiPanel()
	:
	font(L"arial", fontSize)
{
}

void guiPanel::create(vec2 windowSize)
{

	//calcula posições e resoluções
	resolution = windowSize;
	panelRes = vec2(200, windowSize.y);

	const float xFactor = ((windowSize.x - 200.0f) / windowSize.x);
	const vec2 position((xFactor * 2) - 1.0f, -1.0f);
	const vec2 size(1.0f - position.x, 2.f);
	
	vec2 secPos = position + size;
	std::vector<vec2> vertices =
	{
		position,
		vec2(secPos.x, position.y),
		vec2(position.x, secPos.y),
		secPos,
	};
	std::vector<vec2> coords =
	{
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
	};
	std::vector<int> indices =
	{
		0, 2, 1,
		1, 2, 3
	};

	//cria a imagem de fundo
	gfx.fromBlank(panelRes.x, panelRes.y);
	gfx.drawRectangle(vec2(0, 0), vec2(panelRes.x, panelRes.y), color(30, 30, 30, 255));

	tex.create(gfx, false);
	shader2d.create(L"tex2dVS.cso", L"tex2dPS.cso");
	obj.create(shader2d);
	obj.setTexture(&tex, 0);
	obj.loadFromVertexArray(vertices);
	obj.loadFromTexCoordArray(coords);
	obj.setVertexIndices(indices);
	obj.lock();

	//cria fonte
	

}

void guiPanel::addValue(std::wstring name, float* value, bool readOnly)
{
	
	data.push_back(panelValue{ name, value, type::FLOAT, 1u, readOnly });
}

void guiPanel::addValue(std::wstring name, int* value, bool readOnly)
{
	data.push_back(panelValue{ name, value, type::INTEGER, 1u,readOnly });
}

void guiPanel::addValue(std::wstring name, vec3* value, bool readOnly)
{
	data.push_back(panelValue{ name, value, type::FLOAT, 3u,readOnly });
}

bool guiPanel::handleInput(int mouseX, int mouseY, bool clicking)
{
	void* mouseOverValue = nullptr;
	type mouseOverValueType = type::NONE;
	float cursor = 0.0f;
	float xStart = resolution.x - panelRes.x;
	int offset = 0;

	if (mouseX < xStart) return false;

	for (panelValue& i : data)
	{
		float cursorNext = cursor + fontSize + (fontSize + 6.f) * i.arraySize;
		if (i.readOnly == false && mouseY < cursorNext && cursor < mouseY)
		{
			mouseOverValue = i.pValue;
			mouseOverValueType = i.valueType;
			offset = (mouseY - cursor - fontSize) / int(fontSize + 6.0f);
			break;
		}
		cursor = cursorNext;
	}

	if (mouseOverValue && clicking)
	{
		if (lastXMouse != 0)
		{
			lastXMouse += changeValue(
				mouseX - lastXMouse,
				mouseOverValue,
				mouseOverValueType,
				offset
			);
		}
		else
			lastXMouse = mouseX;
	}
	if (!mouseOverValue || !clicking) lastXMouse = 0;

	return mouseOverValue;
}

void guiPanel::draw(Pipeline& pipeline)
{
	gfx.fill(color(40, 40, 40, 255));
	//gfx.drawText(L"oi tudo bem?", font, vec2(0, 0), color(255, 255, 255, 255));
	float cursor = 0.0f;
	for (panelValue& i : data)
	{
		gfx.drawText(i.name, font, vec2(0, cursor), color(255, 255, 255, 255));
		cursor += fontSize;
		//gfx.drawText(std::to_wstring(*i.value), font, vec2(20, cursor), color(255, 255, 255, 255));
		for(int offset = 0; offset < i.arraySize; offset++)
			cursor = drawElement(i, cursor, offset);
	}

	//draws image
	tex.update(gfx);
	pipeline.drawObject(obj);
}

float guiPanel::drawElement(panelValue& val, float cursor, int pointerOffset)
{
	switch (val.valueType)
	{
	case type::FLOAT:
			if (val.readOnly == false)
				gfx.drawRectangle(
					vec2(20, cursor),
					vec2(100.f, fontSize + 6.f),
					color(30, 30, 30, 255)
				);
			gfx.drawText(
			std::to_wstring(reinterpret_cast<float*>(val.pValue)[pointerOffset]),
				font,
				vec2(20, cursor + 3.f),
				color(200, 200, 200, 255)
			);
		cursor += fontSize + 6.f;
		break;
	case type::INTEGER:
		if (val.readOnly == false)
			gfx.drawRectangle(
				vec2(20, cursor),
				vec2(100.f, fontSize + 6.f),
				color(30, 30, 30, 255)
			);
			gfx.drawText(
			std::to_wstring(reinterpret_cast<int*>(val.pValue)[pointerOffset]),
				font,
				vec2(20, cursor + 3.f),
				color(200, 200, 200, 255)
			);
		cursor += fontSize + 6.f;
		break;
	}
	
	return cursor;
}

int guiPanel::changeValue(int mouseDiff, void* value, type valueType, int offset)
{
	switch (valueType)
	{
	case guiPanel::type::NONE:
		return 0;
		break;
	case guiPanel::type::FLOAT:
		reinterpret_cast<float*>(value)[offset] += float(mouseDiff) / 10.0f;
		return mouseDiff;
		break;
	case guiPanel::type::INTEGER:
		reinterpret_cast<int*>(value)[offset] += mouseDiff;
		return mouseDiff;
		break;
	default:
		return 0;
	}
	return 0;
}
