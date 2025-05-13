#include "hudElement.h"

void hudElement::create(Image& img)
{
	std::vector<vec2> vertices =
	{
		vec2(-1.0f, -1.0f),
		vec2(1.0f, -1.0f),
		vec2(-1.0f, 1.0f),
		vec2(1.0f, 1.0f),
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
