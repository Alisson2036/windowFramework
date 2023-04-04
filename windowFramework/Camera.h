#pragma once
#include <DirectXMath.h>


class Camera
{
public:
	Camera() = default;

	Camera(float _position[3], float _angle[2]);

	void setPositionAndAngle(float _position[3], float _angle[2]);

	void move(float _position[3], float _angle[2]);
	void movePosition(float x, float y, float z);
	void moveAngle(float x, float y);
	
	DirectX::XMMATRIX getMatrix();
	static DirectX::XMMATRIX getProjectionMatrix();


private:
	DirectX::XMMATRIX getRotationMatrix();

	float position[3];
	float angle[2];


};

