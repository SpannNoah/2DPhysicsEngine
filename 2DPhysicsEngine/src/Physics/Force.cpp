#include "Force.h"
#include <algorithm>
// Fd = 1/2 * density * coefficient of drag * cross sectional area * ||v||^2 * -v.normalized
// We simplify this to Fd = k * ||v||^2 * -v.normalized bc we're playing pretend
Vec2 Force::GenerateDragForce(const Body& Body, float k)
{
    Vec2 dragForce = Vec2(0.0f, 0.0f);

    if (Body.velocity.MagnitudeSquared() > 0.0f)
    {
        Vec2 dragDirection = Body.velocity.Normalized() * -1.0f;

        float dragMagnitude = k * Body.velocity.MagnitudeSquared();

        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body& Body, float k)
{
    Vec2 frictionForce = Vec2(0, 0);

    Vec2 frictionDirection = Body.velocity.Normalized() * -1.0f;

    float frictionMagnitude = k;

    frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance)
{
    Vec2 distance = b.position - a.position;
    Vec2 forceDirection = distance.Normalized();
    
    // clamping so that it looks better for game physics
    float distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);

    float forceStrength = G * (a.mass * b.mass / distanceSquared);

    return forceDirection * forceStrength;
}

Vec2 Force::GenerateSpringForce(const Body& Body, Vec2 anchor, float restLength, float k)
{
    Vec2 distanceVector = Body.position - anchor;

    float displacement = distanceVector.Magnitude() - restLength;

    return distanceVector.Normalized() * (- k * displacement);
}

Vec2 Force::GenerateSpringForce(const Body& BodyA, const Body& BodyB, float restLength, float k)
{
    Vec2 distanceVectore = BodyA.position - BodyB.position;

    float displacement = distanceVectore.Magnitude() - restLength;

    return distanceVectore.Normalized() * (- k * displacement);
}
