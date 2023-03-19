#pragma once
#include <DirectXMath.h>
#include "Camera.h"

class Instance
{
public:
	Instance(float _position[3], float _angle[3]);

	static void initializeCamera(Camera* _cam);

	void update(float _position[3], float _angle[3]);
	
	void move(float _position[3], float _angle[3]);

	DirectX::XMMATRIX getMatrix();


private:
	float position[3];
	float angle[3];
};