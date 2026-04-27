#include "Contact.h"

void Contact::ResolvePenetration()
{
	if (a->IsStatic() && b->IsStatic())
	{
		return;
	}

	// we us inverse of the math so that if the object has infinite mass we dont change the position
	float da = depth / (a->invMass + b->invMass) * a->invMass;
	float db = depth / (a->invMass + b->invMass) * b->invMass;

	a->position -= normal * da;
	b->position += normal * db;
}

void Contact::ResolveCollision()
{
	ResolvePenetration();

	// Define elasticity
	float e = std::min(a->restitution, b->restitution);

	// Calculate relative velocity
	const Vec2 vRel = (a->velocity - b->velocity);

	// Calculate relative velocity along the normal
	float vRelDotNormal = vRel.Dot(normal);

	// Calculate impulse
	const Vec2 direction = normal;
	const float magnitude = -(1 + e) * vRelDotNormal / (a->invMass + b->invMass);

	Vec2 j = direction * magnitude;

	a->ApplyImpulse(j);
	b->ApplyImpulse(-j);
}
