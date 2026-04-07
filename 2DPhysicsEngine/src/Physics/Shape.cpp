#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius)
{
	this->radius = radius;
}

CircleShape::~CircleShape()
{
	std::cout << "Circle Shape Destructor Called" << std::endl;
}

ShapeType CircleShape::GetType() const
{
	return ShapeType::CIRCLE;
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(radius);
}

float CircleShape::GetMomentOfInertia() const
{
	//tex: Solid Disk Moment of Inertia
	//$${\displaystyle I_{z}={\frac {1}{2}}mr^{2}\,\!}$$
	//Still need to multiply by rigidbodys mass
	
	return .5f * (radius * radius);
}

PolygonShape::PolygonShape(std::vector<Vec2> vertices)
{
	// TODO: ...
}

PolygonShape::~PolygonShape()
{
	// TODO: ...
}

ShapeType PolygonShape::GetType() const
{
	return ShapeType::POLYGON;
}
Shape* PolygonShape::Clone() const
{
	return new PolygonShape(localVertices);
}
float PolygonShape::GetMomentOfInertia() const
{
	//TODO: ...
	return 0.0f;
}
void PolygonShape::UpdateVertices(float angle, const Vec2& position)
{
	// Loop all vertices transforming from local to world space
	for (int i = 0; i < localVertices.size(); i++)
	{
		// Rotate first because rotation needs to be done around the origin then translate
		worldVertices[i] = localVertices[i].Rotate(angle);
		worldVertices[i] += position;
	}
}

BoxShape::BoxShape(float width, float height)
{
	this->width = width;
	this->height = height;

	// Load vertices of box polygon
	localVertices.push_back(Vec2(-width / 2.0f, -height / 2.0f));
	localVertices.push_back(Vec2(width / 2.0f, -height / 2.0f));
	localVertices.push_back(Vec2(width / 2.0f, height / 2.0f));
	localVertices.push_back(Vec2(-width / 2.0f, height / 2.0f));

	worldVertices.push_back(Vec2(-width / 2.0f, -height / 2.0f));
	worldVertices.push_back(Vec2(width / 2.0f, -height / 2.0f));
	worldVertices.push_back(Vec2(width / 2.0f, height / 2.0f));
	worldVertices.push_back(Vec2(-width / 2.0f, height / 2.0f));

}

BoxShape::~BoxShape()
{
	// TODO: ...
}

ShapeType BoxShape::GetType() const
{
	return ShapeType::BOX;
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(width, height);
}

float BoxShape::GetMomentOfInertia() const
{
	//tex: Formula for a box's Moment of Inertia
	//$${\displaystyle I_{c}={\frac {1}{12}}m\left(h^{2}+w^{2}\right)\,\!}$$
	//This still needs to be multiplied by the rigidbodys mass
	
	return .083333 * (width * width + height * height);
}
