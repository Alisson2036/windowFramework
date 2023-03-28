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
	DirectX::XMMATRIX getProjectionMatrix();


private:
	DirectX::XMMATRIX getRotationMatrix();

	float position[3];
	float angle[2];

	const DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.0f);

};

