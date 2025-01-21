#include "hudElement.h"

void hudElement::create(Texture& tex)
{
	std::vector<vec2> vertices =
	{
		vec2(0.0f, -1.0f),
		vec2(1.0f, -1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
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


	shader2d.create(L"tex2dVS.cso", L"tex2dPS.cso");
	obj.create(shader2d);
	obj.setTexture(&tex, 0);
	obj.loadFromVertexArray(vertices);
	obj.loadFromTexCoordArray(coords);
	obj.setVertexIndices(indices);
	obj.lock();

	resolution = tex.getResolution();
}

void hudElement::setPosition(vec2 pos)
{
	obj.set({pos.x, pos.y, 0.f}, {0.f, 0.f, 0.f});
}

void hudElement::draw(Pipeline& pipeline)
{
	pipeline.bind(obj);
}
