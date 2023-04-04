#pragma once
#include <DirectXMath.h>

class Instance
{
public:
	Instance(float _position[3], float _angle[3]);

	void update(float _position[3], float _angle[3]);
	
	void move(float _position[3], float _angle[3]);

	DirectX::XMMATRIX getMatrix();

private:
	float position[3];
	float angle[3];
};