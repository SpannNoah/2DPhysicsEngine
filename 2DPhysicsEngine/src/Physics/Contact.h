#ifndef CONTACT_H

#include "Vec2.h"
#include "Body.h"

struct Contact
{
	Body* a;
	Body* b;

	Vec2 start;
	Vec2 end;

	Vec2 normal;
	float depth;

	Contact() = default;
	~Contact() = default;
};

#define CONTACT_H
#endif // !CONTACT_H

