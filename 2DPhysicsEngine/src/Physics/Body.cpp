#include "Body.h"
#include <iostream>

Body::Body(const Shape& shape, float x, float y, float mass)
{
	this->shape = shape.Clone();
	this->position = Vec2(x, y);
	this->mass = mass;
	this->velocity = Vec2(0, 0);
	this->acceleration = Vec2(0, 0);
	this->rotation = 0.0f;
	this->angularVelocity = 0.0f;
	this->angularAcceleration = 0.0f;
	this->netForce = Vec2(0, 0);
	this->sumTorque = 0.0f;

	if (mass != 0.0f)
	{
		this->invMass = 1 / mass;
	}
	else
	{
		this->invMass = 0.0f;
	}

	I = shape.GetMomentOfInertia() * mass;
	if (I != 0)
	{
		this->invI = 1.0 / I;
	}
	else
	{
		this->invI = 0.0f;
	}
}

Body::~Body()
{
	delete shape;
	std::cout << "Body Destructor Called" << std::endl;
}

void Body::Update(float dt)
{
	IntegrateAngular(dt);
	IntegrateLinear(dt);

	ShapeType type = shape->GetType();
	if (type == BOX || type == POLYGON)
	{
		PolygonShape* polygonShape = (PolygonShape*)shape;
		polygonShape->UpdateVertices(rotation, position);
	}
}

void Body::IntegrateLinear(float dt)
{
	// F = ma -> a = f/m OR a = f * 1/m
	acceleration = netForce * invMass;

	// v = a * dt
	velocity += acceleration * dt;
	// p = v * dt
	position += velocity * dt;

	ClearForces();
}

void Body::IntegrateAngular(float dt)
{
	// Find angular acceleration based on the torque being applied and moment of inertia
	angularAcceleration = sumTorque * invI;

	// Integrate the angular acceleration to find the new angular velocity
	angularVelocity += angularAcceleration * dt;

	// Integrate the angular velocity to find the new rotation angle
	rotation += angularVelocity * dt;

	// Clear all torque acting on the object before the next physics step
	ClearTorque();
}

void Body::AddForce(const Vec2& force)
{
	netForce += force;
}

void Body::ClearForces()
{
	netForce = Vec2(0.0, 0.0);
}

void Body::AddTorque(float torque)
{
	sumTorque += torque;
}

void Body::ClearTorque()
{
	sumTorque = 0.0f;
}
