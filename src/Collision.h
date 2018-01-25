#pragma once

// Include Guard
#ifndef COLLISION_H_
#define COLLISION_H_

// cMath - STD Math Library
#include <cmath>

// Collision Mesh Objects
//
// These are object that will make up the
//	physical body of your object. Spheres
//	are for Spheres/Circles and Planes can
//	be used for more complex constructions,
//	such as a box/pyramid/complexmesh/etc.

// Sphere Collision Mesh
class SphereCollisionMesh
{
public:
	// Constructors
	SphereCollisionMesh() { radius = 0.0f; x = 0.0f; y = 0.0f; z = 0.0f; }
	SphereCollisionMesh(float _x, float _y, float _z, float _radius) { x = _x; y = _y; z = _z; radius = _radius; }

	// Modification Functions
	void setRadius(float _radius) { radius = _radius; }
	void setPosition(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	// Radius
	float radius;

	// Position
	float x;
	float y;
	float z;
};

// Plane Collision Mesh
class PlaneCollisionMesh
{
public:
	// Constructors
	PlaneCollisionMesh()
	{
		px = 0.0f; py = 0.0f; pz = 0.0f; nx = 0.0f; ny = 0.0f; nz = 0.0f;
	}
	PlaneCollisionMesh(float _px, float _py, float _pz, float _nx, float _ny, float _nz)
	{
		px = _px; py = _py; pz = _pz; nx = _nx; ny = _ny; nz = _nz;
	}

	// Modification Functions
	void setPoint(float x, float y, float z) { px = x; py = y; pz = z; }
	void setNormal(float x, float y, float z) { nx = x; ny = y; nz = z; }


	// Point
	float px;
	float py;
	float pz;

	// Normal
	float nx;
	float ny;
	float nz;
};

// Point Collision Mesh
class PointCollisionMesh
{
public:
	// Constructors
	PointCollisionMesh() { x = 0.0f; y = 0.0f; z = 0.0f; }
	PointCollisionMesh(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	// Modification Functions
	void setPosition(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	float x;
	float y;
	float z;
};

// Collision Test Return(CTR) Objects
//
// These are returned when there respective collision
//	test is run. They should give you any information
//	that is relevant to the collision

struct CTRSpherePlane
{
	SphereCollisionMesh* SCM = NULL;
	PlaneCollisionMesh* PCM = NULL;
	bool CollisionDetected = false;
	bool OnSurface = false;
	float Penetration = 0.0f;
	float nX = 0.0f;
	float nY = 0.0f;
	float nZ = 0.0f;
};

struct CTRSphereSphere
{
	SphereCollisionMesh* SCM1 = NULL;
	SphereCollisionMesh* SCM2 = NULL;
	bool CollisionDetected = false;
	bool OnSurface = false;
	float Penetration = 0.0f;
	float nX = 0.0f;
	float nY = 0.0f;
	float nZ = 0.0f;
};

struct CTRPointPlane
{
	PointCollisionMesh* POCM = NULL;
	PlaneCollisionMesh* PLCM = NULL;
	bool CollisionDetected = false;
	bool OnSurface = false;
	float Penetration = 0.0f;
	float nX = 0.0f;
	float nY = 0.0f;
	float nZ = 0.0f;
};

// Collision Test Functions
//
// These are what you would call when testing whether
//	or not two collision meshes collide

// Sphere-Plane Collision Test
CTRSpherePlane SpherePlaneCollisionTest(SphereCollisionMesh& _SCM, PlaneCollisionMesh& _PCM)
{
	// Set up variables
	float x = _SCM.x; float y = _SCM.y; float z = _SCM.z;
	float radius = _SCM.radius;
	float px = _PCM.px; float py = _PCM.py; float pz = _PCM.pz;
	float nx = _PCM.nx; float ny = _PCM.ny; float nz = _PCM.nz;


	// Calculate Distance
	float nlength = sqrtf(nx * nx + ny * ny + nz * nz);
	float Nx = nx / nlength;
	float Ny = ny / nlength;
	float Nz = nz / nlength;

	float vx = x - px;
	float vy = y - py;
	float vz = z - pz;

	float distance = Nx * vx + Ny * vy + Nz *vz;

	// Calculate Penetration
	float penetration = -(distance - radius);

	// Output Result
	CTRSpherePlane Output;

	// Copy Location of SCMs
	Output.SCM = &_SCM;
	Output.PCM = &_PCM;

	// If there is not penetration
	if (penetration < 0)
	{
		Output.CollisionDetected = false;
		Output.OnSurface = false;
		Output.Penetration = penetration;
		Output.nX = nx;
		Output.nY = ny;
		Output.nZ = nz;
	}
	// If there is penetration
	else if (penetration > 0)
	{
		Output.CollisionDetected = true;
		Output.OnSurface = false;
		Output.Penetration = penetration;
		Output.nX = nx;
		Output.nY = ny;
		Output.nZ = nz;
	}
	// If it rests on the surface
	else
	{
		Output.CollisionDetected = true;
		Output.OnSurface = true;
		Output.Penetration = penetration;
		Output.nX = nx;
		Output.nY = ny;
		Output.nZ = nz;
	}

	// Output Collision Test Results
	return Output;
}

// Sphere-Sphere Collision Test
CTRSphereSphere SphereShereCollisionTest(SphereCollisionMesh& _SCM1, SphereCollisionMesh& _SCM2)
{
	// Set up variables
	float x1 = _SCM1.x;	float y1 = _SCM1.y;	float z1 = _SCM1.z;
	float radius1 = _SCM1.radius;
	float x2 = _SCM2.x;	float y2 = _SCM2.y;	float z2 = _SCM2.z;
	float radius2 = _SCM2.radius;

	// Distance Calculation
	float distance = sqrtf(powf(x2 - x1, 2) + powf(z2 - z1, 2) + powf(z2 - z1, 2));

	// Calculate Penetration
	float penetration = -(distance - (radius1 + radius2));

	// Output Result
	CTRSphereSphere Output;

	// Copy Location of SCMs
	Output.SCM1 = &_SCM1;
	Output.SCM2 = &_SCM2;

	// If there is not penetration
	if (penetration < 0)
	{
		Output.CollisionDetected = false;
		Output.OnSurface = false;
		Output.Penetration = penetration;
	}
	// If there is penetration
	else if (penetration > 0)
	{
		Output.CollisionDetected = true;
		Output.OnSurface = false;
		Output.Penetration = penetration;
	}
	else
	{
		Output.CollisionDetected = true;
		Output.OnSurface = true;
		Output.Penetration = penetration;
	}

	// Output Collision Test Results
	return Output;
}

CTRPointPlane PointPlaneCollision(PointCollisionMesh& _POCM, PlaneCollisionMesh& _PLCM)
{
	// Set up variables
	float x = _POCM.x; float y = _POCM.y; float z = _POCM.z;
	float px = _PLCM.px; float py = _PLCM.py; float pz = _PLCM.pz;
	float nx = _PLCM.nx; float ny = _PLCM.ny; float nz = _PLCM.nz;


	// Calculate Distance
	float nlength = sqrtf(nx * nx + ny * ny + nz * nz);
	float Nx = nx / nlength;
	float Ny = ny / nlength;
	float Nz = nz / nlength;

	float vx = x - px;
	float vy = y - py;
	float vz = z - pz;

	float distance = Nx * vx + Ny * vy + Nz *vz;

	// Calculate Penetration
	float penetration = -(distance);

	// Output Result
	CTRPointPlane Output;

	// Copy Location of SCMs
	Output.POCM = &_POCM;
	Output.PLCM = &_PLCM;

	// If there is not penetration
	if (penetration < 0)
	{
		Output.CollisionDetected = false;
		Output.OnSurface = false;
		Output.Penetration = penetration;
		Output.nX = nx;
		Output.nY = ny;
		Output.nZ = nz;
	}
	// If there is penetration
	else if (penetration > 0)
	{
		Output.CollisionDetected = true;
		Output.OnSurface = false;
		Output.Penetration = penetration;
		Output.nX = nx;
		Output.nY = ny;
		Output.nZ = nz;
	}
	// If it rests on the surface
	else
	{
		Output.CollisionDetected = true;
		Output.OnSurface = true;
		Output.Penetration = penetration;
		Output.nX = nx;
		Output.nY = ny;
		Output.nZ = nz;
	}

	// Output Collision Test Results
	return Output;
}

#endif // ! COLLISION_H_