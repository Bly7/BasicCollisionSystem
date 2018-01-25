#pragma once

#include <cmath>

#include <vector>

#include "Collision.h"

class Physics
{
public:
	// Constructors
	Physics()
	{
		pX = 0.0f;
		pY = 0.0f;
		pZ = 0.0f;
		lpX = 0.0f;
		lpY = 0.0f;
		lpZ = 0.0f;
		vX = 0.0f;
		vY = 0.0f;
		vZ = 0.0f;
	}
	Physics(const float _pX, const float _pY, const float _pZ)
	{
		pX = _pX;
		pY = _pY;
		pZ = _pZ;
		lpX = 0.0f;
		lpY = 0.0f;
		lpZ = 0.0f;
	}
	Physics(const float _pX, const float _pY, const float _pZ, const float _vX, const float _vY, const float _vZ)
	{
		pX = _pX;
		pY = _pY;
		pZ = _pZ;
		lpX = 0.0f;
		lpY = 0.0f;
		lpZ = 0.0f;
		vX = _vX;
		vY = _vY;
		vZ = _vZ;
	}

	// Modification Functions
	void setPosition(const float _pX, const float _pY, const float _pZ)
	{
		pX = _pX;
		pY = _pY;
		pZ = _pZ;
	}
	void setVelocity(const float _vX, const float _vY, const float _vZ)
	{
		vX = _vX;
		vY = _vY;
		vZ = _vZ;
	}

	// Access Functions
	void getPosition(float& _pX, float& _pY, float& _pZ)
	{
		_pX = pX;
		_pY = pY;
		_pZ = pZ;
	}

	void getVelocity(float& _vX, float& _vY, float _vZ)
	{
		_vX = vX;
		_vY = vY;
		_vZ = vZ;
	}

	void getLastPosition(float& _lpX, float& _lpY, float& _lpZ)
	{
		_lpX = lpX;
		_lpY = lpY;
		_lpZ = lpZ;
	}

	// Update Position based upon Velocity and dT
	void Update(float dT)
	{
		lpX = pX;
		lpY = pY;
		lpZ = pZ;

		pX += vX * dT;
		pY += vY * dT;
		pZ += vZ * dT;
	}
public:
	float pX;
	float pY;
	float pZ;

	float lpX;
	float lpY;
	float lpZ;

	float vX;
	float vY;
	float vZ;
};

class Ball
{
public:
	// Constructors
	Ball()
	{
		radius = 1;
		SCM.setRadius(1);
	}
	Ball(const float _pX, const float _pY, const float _pZ,
		const float _vX, const float _vY, const float _vZ,
		const float _radius)
	{
		bPhys = Physics(_pX, _pY, _pZ, _vX, _vY, _vZ);
		radius = _radius;
		SCM.setRadius(radius);
		SCM.setPosition(_pX, _pY, _pZ);
	}

	// Update Function
	void Update(float dT)
	{
		// This updates the Physics Position in the background
		bPhys.Update(dT);


		// Update SCM
		SCM.setPosition(bPhys.pX, bPhys.pY, bPhys.pZ);
	}

	// Modification Function
	void Reverse(const float _nX, const float _nY, const float _nZ)
	{
		// Normalize Normal
		float magnitude = sqrtf((_nX * _nX) + (_nY * _nY) + (_nZ * _nZ));
		float nX = _nX / magnitude;
		float nY = _nY / magnitude;
		float nZ = _nZ / magnitude;

		// if normal > 0 multiply reverse the normal
		// if normal < 0 multiply by the normal
		// if normal == 0 keep it the same

		// Update vX
		if (nX > 0)
			bPhys.vX *= -nX;
		else if (nX < 0)
			bPhys.vX *= nX;
		else
			bPhys.vX == bPhys.vX;

		// Update vY
		if (nY > 0)
			bPhys.vY *= -nY;
		else if (nY < 0)
			bPhys.vY *= nY;
		else
			bPhys.vY == bPhys.vY;

		// Update vZ
		if (nZ > 0)
			bPhys.vZ *= -nZ;
		else if (nZ < 0)
			bPhys.vZ *= nZ;
		else
			bPhys.vZ == bPhys.vZ;
	}

	void setPosition(const float _pX, const float _pY, const float _pZ)
	{
		bPhys.setPosition(_pX, _pY, _pZ);
		SCM.setPosition(_pX, _pY, _pZ);
	}

	void setVelocity(const float _vX, const float _vY, const float _vZ)
	{
		bPhys.setVelocity(_vX, _vY, _vZ);
	}

	// Access Functions
	void getPosition(float& _pX, float& _pY, float& _pZ)
	{
		bPhys.getPosition(_pX, _pY, _pZ);
	}

	void getVelocity(float& _vX, float& _vY, float _vZ)
	{
		bPhys.getVelocity(_vX, _vY, _vZ);
	}

	float getRadius()
	{
		return radius;
	}
public:
	SphereCollisionMesh SCM;
	Physics bPhys;

	float radius;
private:

};

class Paddle
{
public:
	// Constructors
	Paddle()
	{
		
	}
	Paddle(const float _pX, const float _pY, const float _pZ, const float _Width, const float _Height)
	{
		PCM[0] = PlaneCollisionMesh(_pX, _pY - _Height / 2, _pZ, 0, -1, 0);
		PCM[1] = PlaneCollisionMesh(_pX + _Width / 2, _pY, _pZ, 1, 0, 0);
		PCM[2] = PlaneCollisionMesh(_pX, _pY + _Height / 2, _pZ, 0, 1, 0);
		PCM[3] = PlaneCollisionMesh(_pX - _Width / 2, _pY, _pZ, -1, 0, 0);

		Width = _Width;
		Height = _Height;

		pPhys.setPosition(_pX, _pY, _pZ);
	}

	// Modification Functions
	void MoveUp(const float amount)
	{
		float x, y, z;
		pPhys.getPosition(x, y, z);

		y -= amount;

		pPhys.setPosition(x, y, z);

		PCM[0] = PlaneCollisionMesh(x, y - Height / 2, z, 0, -1, 0);
		PCM[1] = PlaneCollisionMesh(x + Width / 2, y, z, 1, 0, 0);
		PCM[2] = PlaneCollisionMesh(x, y + Height / 2, z, 0, 1, 0);
		PCM[3] = PlaneCollisionMesh(x - Width / 2, y, z, -1, 0, 0);
	}
	void MoveDown(const float amount)
	{
		float x, y, z;
		pPhys.getPosition(x, y, z);

		y += amount;

		pPhys.setPosition(x, y, z);

		PCM[0] = PlaneCollisionMesh(x, y - Height / 2, z, 0, -1, 0);
		PCM[1] = PlaneCollisionMesh(x + Width / 2, y, z, 1, 0, 0);
		PCM[2] = PlaneCollisionMesh(x, y + Height / 2, z, 0, 1, 0);
		PCM[3] = PlaneCollisionMesh(x - Width / 2, y, z, -1, 0, 0);
	}

public:
	PlaneCollisionMesh PCM[4];

	float Width;
	float Height;

	Physics pPhys;
};

class World
{
public:
	World(const PlaneCollisionMesh _Top, const PlaneCollisionMesh _Bot, const PlaneCollisionMesh _Left, const PlaneCollisionMesh _Right)
	{
		PCM[0] = _Top;
		PCM[1] = _Right;
		PCM[2] = _Bot;
		PCM[3] = _Left;
	}

	PlaneCollisionMesh PCM[4];
};

std::vector<CTRSpherePlane> BallWorldCollisionHandler(Ball& ball, World& world)
{
	std::vector<CTRSpherePlane> out;
	out.resize(4);

	for (int i = 0; i < 4; i++)
	{
		out[i] = SpherePlaneCollisionTest(ball.SCM, world.PCM[i]);

		// World Collision Handle
		if (out[i].CollisionDetected)
		{
			float x, y, z; // bpos
			float nx, ny, nz; // cnor
			float pen; // cpen
			ball.getPosition(x, y, z);
			nx = out[i].nX;
			ny = out[i].nY;
			nz = out[i].nZ;
			pen = out[i].Penetration;

			x += (nx * pen * 1.1f);
			y += (ny * pen * 1.1f);
			z += (nz * pen * 1.1f);

			ball.setPosition(x, y, z);

			ball.Reverse(nx, ny, nz);
		}
	}

	return out;
}

void BallPaddleCollisionHandler(Ball& ball, Paddle& paddle)
{
	CTRSpherePlane ctrsp[4];
	int wallspenetrated = 0;
	int left;
	int right;

	for (int i = 0; i < 4; i++)
	{
		ctrsp[i] = SpherePlaneCollisionTest(ball.SCM, paddle.PCM[i]);

		if (ctrsp[i].CollisionDetected)
		{
			wallspenetrated++;
		}
	}

	if (wallspenetrated == 4)
	{
		float lastx;
		float lasty;
		float lastz;
		ball.bPhys.getLastPosition(lastx, lasty, lastz);

		CTRSpherePlane ctrlsp;
		SphereCollisionMesh tSCM(lastx, lasty, lastz, ball.radius);

		int wallpenetrated = 0;

		for (int i = 0; i < 4; i++)
		{
			ctrlsp = SpherePlaneCollisionTest(tSCM, paddle.PCM[i]);

			if (!ctrlsp.CollisionDetected)
			{
				wallpenetrated = i;
				break;
			}
		}

		float x, y, z; // bpos
		float nx, ny, nz; // cnor
		float pen; // cpen
		ball.getPosition(x, y, z);
		nx = ctrsp[wallpenetrated].nX;
		ny = ctrsp[wallpenetrated].nY;
		nz = ctrsp[wallpenetrated].nZ;
		pen = ctrsp[wallpenetrated].Penetration;

		x += (nx * pen * 1.1f);
		y += (ny * pen * 1.1f);
		z += (nz * pen * 1.1f);

		ball.setPosition(x, y, z);

		ball.Reverse(nx, ny, nz);
	}
}