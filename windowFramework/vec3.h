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
	void operator+=(vec3 rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	void operator-=(vec3 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	vec3 operator*(float rhs)
	{
		vec3 novo(*this);
		novo.x *= rhs;
		novo.y *= rhs;
		novo.z *= rhs;
		return novo;
	}
	vec3 operator/(float rhs)
	{
		vec3 novo(*this);
		novo.x /= rhs;
		novo.y /= rhs;
		novo.z /= rhs;
		return novo;
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

	//dot product
	float operator*(const vec3& a) {
		return a.x * x + a.y * y + a.z * z;
	}
	static float dot(const vec3& a, const vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	//cross product
	static vec3 cross(const vec3& a, const vec3& b) {
		return vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}
	//length
	float lengthSquared()
	{
		return x * x + y * y + z * z;
	}


	float x;
	float y;
	float z;

};