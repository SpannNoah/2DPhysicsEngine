#include "CollisionDetection.h"
#include "../Graphics.h"
#include <limits.h>

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;
    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(a, b, contact);
    }
    if (aIsPolygon && bIsPolygon)
    {
        return IsCollidingPolygonPolygon(a, b, contact);
    }
    if (aIsPolygon && bIsCircle)
    {
        return IsCollidingPolygonCircle(a, b, contact);
    }
    if (aIsCircle && bIsPolygon)
    {
        return IsCollidingPolygonCircle(b, a, contact);
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

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
    const PolygonShape* polygonA = (PolygonShape*)a->shape;
    const PolygonShape* polygonB = (PolygonShape*)b->shape;
    Vec2 aAxis, bAxis;
    Vec2 aPoint, bPoint;

    float abSeparation = polygonA->FindMinimumSeparation(polygonB, aAxis, aPoint);
    float baSeparation = polygonB->FindMinimumSeparation(polygonA, bAxis, bPoint);
    if (abSeparation >= 0 || baSeparation >= 0)
    {
        return false;
    }

    contact.a = a;
    contact.b = b;
    if (abSeparation > baSeparation)
    {
        contact.depth = -abSeparation;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = aPoint + contact.normal * contact.depth;
    }
    else
    {
        contact.depth = -baSeparation;
        contact.normal = -bAxis.Normal();
        contact.start = bPoint - contact.normal * contact.depth;;
        contact.end = bPoint;
    }
    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* polygon, Body* circle, Contact& contact)
{
    PolygonShape* polygonShape = (PolygonShape*)polygon->shape;
    CircleShape* circleShape = (CircleShape*)circle->shape;

    bool isOutside = false;
    Vec2 minCurrentVertex;
    Vec2 minNextVertex;
    float distanceCircleToEdge = std::numeric_limits<float>::lowest();

    for (int i = 0; i < polygonShape->worldVertices.size(); i++)
    {
        int currVertex = i;
        int nextVertex = (i + 1) % polygonShape->worldVertices.size();
        Vec2 edge = polygonShape->EdgeAt(currVertex);
        Vec2 normal = edge.Normal();

        Vec2 edgeToCircle = circle->position - polygonShape->worldVertices[currVertex];
        float projection = edgeToCircle.Dot(normal);

        if (projection > 0)
        {
            distanceCircleToEdge = projection;
            minCurrentVertex = polygonShape->worldVertices[currVertex];
            minNextVertex = polygonShape->worldVertices[nextVertex];
            isOutside = true;
            break;
        }
        else
        {
            if (projection > distanceCircleToEdge)
            {
                distanceCircleToEdge = projection;
                minCurrentVertex = polygonShape->worldVertices[currVertex];
                minNextVertex = polygonShape->worldVertices[nextVertex];
            }
        }
    }

    if (isOutside)
    {
        Vec2 v1 = circle->position - minCurrentVertex;
        Vec2 v2 = minNextVertex - minCurrentVertex;
        if (v1.Dot(v2) < 0)
        {
            if (v1.Magnitude() > circleShape->radius)
            {
                return false;
            }
            else
            {
                contact.a = polygon;
                contact.b = circle;
                contact.depth = circleShape->radius - v1.Magnitude();
                contact.normal = v1.Normalize();
                contact.start = circle->position + (contact.normal * -circleShape->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        }
        else
        {
            v1 = circle->position - minNextVertex;
            v2 = minCurrentVertex - minNextVertex;
            if (v1.Dot(v2) < 0)
            {
                if (v1.Magnitude() > circleShape->radius)
                {
                    return false;
                }
                else
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - v1.Magnitude();
                    contact.normal = v1.Normalize();
                    contact.start = circle->position + (contact.normal * -circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            }
            else
            {
                if (distanceCircleToEdge > circleShape->radius)
                {
                    return false;
                }
                else
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - distanceCircleToEdge;
                    contact.normal = (minNextVertex - minCurrentVertex).Normal();
                    contact.start = circle->position - (contact.normal * circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            }
        }
    }
    else
    {
        contact.a = polygon;
        contact.b = circle;
        contact.depth - circleShape->radius - distanceCircleToEdge;
        contact.normal = (minNextVertex - minCurrentVertex).Normal();
        contact.start = circle->position - (contact.normal * circleShape->radius);
        contact.end = contact.start + (contact.normal * contact.depth);
    }

    return true;
}
