#include "hudElement.h"

void hudElement::create(Image& img)
{
	create(img, vec2(-1.f, -1.f), vec2(2.f, 2.f));
}

void hudElement::create(Image& img, vec2 position, vec2 size)
{
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

	tex.create(img, false);
	shader2d.create(L"tex2dVS.cso", L"tex2dPS.cso");
	obj.create(shader2d);
	obj.setTexture(&tex, 0);
	obj.loadFromVertexArray(vertices);
	obj.loadFromTexCoordArray(coords);
	obj.setVertexIndices(indices);
	obj.lock();

	resolution = tex.getResolution();
}

void hudElement::draw(Pipeline& pipeline)
{
	pipeline.drawObject(obj);
}

void hudElement::update(Image& img)
{
	tex.update(img);


	resolution = tex.getResolution();
	
	// resolution = tex.getResolution();

}
