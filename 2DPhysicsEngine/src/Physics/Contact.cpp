#include "Contact.h"

void Contact::ResolvePenetration()
{
	if (a->IsStatic() && b->IsStatic())
	{
		return;
	}

	// we us inverse of the mass so that if the object has infinite mass we dont change the position
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
	Vec2 ra = end - a->position;
	Vec2 rb = start - b->position;
	Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
	Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
	const Vec2 vRel = va - vb;

	// Calculate relative velocity along the normal
	float vRelDotNormal = vRel.Dot(normal);

	// Calculate impulse
	const Vec2 direction = normal;
	const float magnitude = -(1 + e) * vRelDotNormal / (a->invMass + b->invMass);

	Vec2 j = direction * magnitude;

	a->ApplyImpulse(j);
	b->ApplyImpulse(-j);
}
