#pragma once
#include <DirectXMath.h>


class Camera
{
public:
	Camera() = default;

	Camera(float _position[3], float _angle[3]);

	void setPositionAndAngle(float _position[3], float _angle[3]);

	void move(float _position[3], float _angle[3]);
	void movePosition(float x, float y, float z);
	void moveAngle(float x, float y, float z);

	DirectX::XMMATRIX getMatrix();


private:
	float position[3];
	float angle[3];

};

