#include "physicsDomain.h"

void physicsDomain::addObject(physicsObject* obj)
{
	objects.push_back(obj);
}

void physicsDomain::setGravity(vec3 grav)
{
	gravity = grav;
}

vec3 physicsDomain::cubeColliding(vec3 pos, vec3 rot, vec3 scale1, vec3 scale2)
{    
    // Compute the rotation matrix components
    DirectX::XMMATRIX rotMatrix(
        DirectX::XMMatrixRotationX(rot.x) *
        DirectX::XMMatrixRotationY(rot.y) *
        DirectX::XMMatrixRotationZ(rot.z));

    // Compute the rotated axes of the second cube (columns of the rotation matrix)
    vec3 u1, u2, u3;
    {
        auto a = rotMatrix.r[0].m128_f32;
        u1.x = a[0];
        u1.y = a[1];
        u1.z = a[2];
        auto b = rotMatrix.r[1].m128_f32;
        u2.x = b[0];
        u2.y = b[1];
        u2.z = b[2];
        auto c = rotMatrix.r[2].m128_f32;
        u3.x = c[0];
        u3.y = c[1];
        u3.z = c[2];
    }

    // Collect all separating axes
    std::vector<vec3> axes;

    // World axes (x, y, z)
    axes.emplace_back(1, 0, 0);
    axes.emplace_back(0, 1, 0);
    axes.emplace_back(0, 0, 1);

    // OBB axes (u1, u2, u3)
    axes.push_back(u1);
    axes.push_back(u2);
    axes.push_back(u3);

    // Cross products of world axes and OBB axes
    const vec3 worldAxes[3] = { vec3(1,0,0), vec3(0,1,0), vec3(0,0,1) };
    const vec3 obbAxes[3] = { u1, u2, u3 };

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            vec3 crossProd = vec3::cross(worldAxes[i], obbAxes[j]);
            if (crossProd.lengthSquared() > 1e-6f) { // Skip zero vectors
                crossProd.normalize();
                axes.push_back(crossProd);
            }
        }
    }

    // Check each axis for overlap
    float minDiff = 100000.0f;
    vec3 minAxis(0.f,0.f,0.f);
    for (const vec3& axis : axes) {
        // Projection for AABB (centered at (0,0,0) with half-extent 1)
        float aabbHalf = 
            scale1.x * std::abs(axis.x) + 
            scale1.y * std::abs(axis.y) +
            scale1.z * std::abs(axis.z);
        float aabbMin = -aabbHalf;
        float aabbMax = aabbHalf;

        // Projection for OBB (centered at 'position' with half-extent 1)
        float obbCenterProj = vec3::dot(pos, axis);
        float obbHalf = 
            scale2.x * std::abs(vec3::dot(u1, axis)) +
            scale2.y * std::abs(vec3::dot(u2, axis)) +
            scale2.z * std::abs(vec3::dot(u3, axis));
        float obbMin = obbCenterProj - obbHalf;
        float obbMax = obbCenterProj + obbHalf;

        // Check for separating axis
        if (obbMax < aabbMin || obbMin > aabbMax) {
            return vec3(0.f, 0.f, 0.f);
        }
        if (aabbMax - obbMin < minDiff) 
        {
            minDiff = aabbMax - obbMin;
            minAxis = axis;
        }
        if (obbMax - aabbMin < minDiff) 
        {
            minDiff = obbMax - aabbMin;
            minAxis = vec3(0.f, 0.f, 0.f) - axis;
        }
    }

    return minAxis * minDiff;
}

void physicsDomain::solve(float dt)
{
	const vec3 boxPos(-10.0f, 20.0f, -10.0f);
	const vec3 boxSize(20.f, -20.f, 20.f);

	//solve verlet for all objects
	//and contain inside box
	for (physicsObject* i : objects)
	{
		i->solveVerlet(gravity, dt);
		i->containInBox(boxPos, boxSize);
	}

	//collisions
	for(int i = 0; i < objects.size(); i++)
		for (int j = i + 1; j < objects.size(); j++)
			objects[i]->collide(objects[j]);
}
