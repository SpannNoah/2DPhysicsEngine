#ifndef BODY_H
#define BODY_H

#include <SDL.h>
#include <SDL_image.h>
#include "Vec2.h"
#include "Shape.h"

struct Body
{
	bool IsColliding = false;

	// Linear motion, linear force, mass
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 netForce;
	
	// Angular motion
	float rotation;
	float angularVelocity;
	float angularAcceleration;
	float sumTorque;

	// Mass and Moment of Inertia
	float mass;
	float invMass;
	float I;
	float invI;

	// Coefficient of Restitution (elasticity)
	float restitution;
	float friction;

	// Pointer to geometry of this rigid body
	Shape* shape = nullptr;

	// Pointer to SDL texture
	SDL_Texture* texture = nullptr;

	Body(const Shape& shape, float x, float y, float mass);
	~Body();

	bool IsStatic() const;

	void Update(float dt);

	void IntegrateLinear(float dt);
	void IntegrateAngular(float dt);

	void AddForce(const Vec2& force);
	void ClearForces();

	void AddTorque(float torque);
	void ClearTorque();

	void ApplyImpulse(const Vec2& j);
	void ApplyImpulse(const Vec2& j, const Vec2& r);

	void SetTexture(const char* fileName);
};
#endif