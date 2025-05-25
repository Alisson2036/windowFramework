#include "guiPanel.h"

void guiPanel::create(vec2 windowSize)
{

	//calcula posições e resoluções
	panelRes = vec2(windowSize.x / 4, windowSize.y);

	const vec2 position(0.5f, -1.0f);
	const vec2 size(0.5f, 2.f);
	
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

	resolution = tex.getResolution();
}

void guiPanel::draw(Pipeline& pipeline)
{
	pipeline.drawObject(obj);
}
