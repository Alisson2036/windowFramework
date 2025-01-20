#pragma once

class vec3
{
public:
	//construtores padrao
	vec3() = default;
	vec3(const float X, const float Y, const float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	vec3(const vec3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}

	//operadores
	vec3 operator+(vec3 rhs) const
	{
		return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	vec3 operator-(vec3 rhs) const
	{
		return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	vec3 operator*(vec3 rhs) const
	{
		return vec3(x * rhs.x, y * rhs.y, z * rhs.z);
	}
	vec3 operator/(vec3 rhs) const
	{
		return vec3(x / rhs.x, y / rhs.y, z / rhs.z);
	}
	vec3 operator+=(vec3 rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	vec3 operator-=(vec3 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	vec3 operator*=(vec3 rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
	}
	vec3 operator/=(vec3 rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
	}




	float x;
	float y;
	float z;

};