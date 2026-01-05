#include "sprite.h"

void sprite::create(Image& img)
{
	create(img, vec2(-1.f, -1.f), vec2(2.f, 2.f));
}

void sprite::create(Image& img, vec2 position, vec2 size)
{
	Texture* pTex = new Texture();
	pTex->create(img, false);
	create(
		pTex,
		position,
		size
	);
}

void sprite::create(Texture* img, vec2 position, vec2 size)
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
	std::vector<UINT> indices =
	{
		0, 2, 1,
		1, 2, 3
	};

	tex = img;
	shader2d.create(L"CompiledShaders\\tex2dVS.cso", L"CompiledShaders\\tex2dPS.cso");
	obj.create(&shader2d);
	obj.setTexture(tex, 0);
	obj.loadFromVertexArray(vertices);
	obj.loadFromTexCoordArray(coords);
	obj.setVertexIndices(indices);
	obj.lock();

	resolution = tex->getResolution();
}

void sprite::draw(Pipeline& pipeline)
{
	pipeline.drawObject(obj);
}

void sprite::update(Image& img)
{
	tex->update(img);


	resolution = tex->getResolution();
	
	// resolution = tex.getResolution();

}
