#include "light.h"

Light::Light()
{

	DirectX::XMVECTOR b[] = {
		DirectX::XMVECTOR{0.0f,0.0f,0.0f}
	};
	buf.create(
		b,
		1,
		sizeof(DirectX::XMVECTOR)
	);
}

void Light::updatePos(vec3 Position)
{
	DirectX::XMVECTOR b[] = {
		DirectX::XMVECTOR{Position.x, Position.y, Position.z}
	};

	buf.update(b);
}

void Light::bind(int bufferSlot)
{
	buf.setSlot(bufferSlot);
	buf.bind();
}
