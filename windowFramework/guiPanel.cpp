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

void guiPanel::draw(Pipeline& pipeline)
{
	gfx.drawText(L"oi tudo bem?", font, vec2(0, 0), color(255, 255, 255, 255));

	//draws image
	tex.update(gfx);
	pipeline.drawObject(obj);
}
