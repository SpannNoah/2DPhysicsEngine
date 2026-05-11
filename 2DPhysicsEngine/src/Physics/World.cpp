#include "World.h"
#include "Constants.h"
#include "CollisionDetection.h"
#include <iostream>

World::World(float gravity)
{
	G = -gravity;
	std::cout << "World Constructor Called" << std::endl;
}

World::~World()
{
	for (auto body : bodies)
	{
		delete body;
	}
	std::cout << "World Destructor Called" << std::endl;
}

void World::Update(float dt)
{
	for (auto body : bodies)
	{
		Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
		body->AddForce(weight);

		for (auto force : forces)
		{
			body->AddForce(force);
		}

		for (auto torque : torques)
		{
			body->AddTorque(torque);
		}
	}

	for (auto body : bodies)
	{
		body->Update(dt);
	}

	CheckCollisions();
}

void World::AddBody(Body* body)
{
	bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies()
{
	return bodies;
}

void World::AddForce(const Vec2 force)
{
	forces.push_back(force);
}

void World::AddTorque(const float torque)
{
	torques.push_back(torque);
}

void World::CheckCollisions()
{
	for (int i = 0; i <= bodies.size() - 1; i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body* a = bodies[i];
			Body* b = bodies[j];

			a->IsColliding = false;
			b->IsColliding = false;

			Contact contact;
			if (CollisionDetection::IsColliding(a, b, contact))
			{
				contact.ResolveCollision();
			}
		}
	}
}
