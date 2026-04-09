#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(a, b, contact);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact)
{
    const CircleShape* circleA = (CircleShape*)a->shape;
    const CircleShape* circleB = (CircleShape*)b->shape;

    const float radiiSum = circleA->radius + circleB->radius;
    const Vec2 ab = b->position - a->position;

    bool isColliding = ab.MagnitudeSquared() <= (radiiSum * radiiSum);

    if (!isColliding)
    {
        return false;
    }
    
    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.start = b->position - contact.normal * circleB->radius;
    contact.end = a->position + contact.normal * circleA->radius;
    contact.depth = (contact.end - contact.start).Magnitude();
    return true;
}
