#include "light.h"

Light::Light()
{
	struct pos
	{
		float x;
		float y;
		float z;
	}; 
	DirectX::XMVECTOR b[] = {
		DirectX::XMVECTOR{0.0f,1.0f,0.0f}
	};
	buf.create(
		b,
		1,
		sizeof(DirectX::XMVECTOR)
	);
}

void Light::updatePos(float _pos[3])
{
	DirectX::XMVECTOR b[] = {
		DirectX::XMVECTOR{_pos[0], _pos[1], _pos[2]}
	};

	buf.update(b);
}

void Light::bind(int bufferSlot)
{
	buf.setSlot(bufferSlot);
	buf.bind();
}
