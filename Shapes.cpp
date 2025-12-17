#include "Shapes.h"
#include "glm/glm.hpp"
#include <iostream>

//mtv is minimum translation vector

bool CheckCircleCircleCol(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB)
{
	/* Getting the distance between the circle's centers*/ 
	/* using the Pythagorean Theorem to calculate the distance*/
	float distX = PosA.x - PosB.x;
	float distY = PosA.y - PosB.y;
	float distance = sqrt((distX * distX) + (distY * distY));

	/* If the distance is less than the sum of the circle's radii, the circles are touching*/
	if (distance <= CircleA.Radius + CircleB.Radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CheckAABBCol(const glm::vec2& PosA, const AABB& AABBA, const glm::vec2& PosB, const AABB& AABBB)
{
	return PosA.x - AABBA.HalfExtents.x < PosB.x + AABBB.HalfExtents.x && // LEFT within RIGHT
		PosA.x + AABBA.HalfExtents.x > PosB.x - AABBB.HalfExtents.x && // RIGHT within LEFT
		PosA.y - AABBA.HalfExtents.y < PosB.y + AABBB.HalfExtents.y && // TOP within BOTTOM
		PosA.y + AABBA.HalfExtents.y > PosB.y - AABBB.HalfExtents.y; // RIGHT within TOP
}

bool CheckCircleAABBCol(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& AABBB)
{
	/* find the nearest point in the AABB in the direction of the circle*/
	float distX = PosA.x - glm::clamp(PosA.x, PosB.x - AABBB.HalfExtents.x, PosB.x + AABBB.HalfExtents.x);
	float distY = PosA.y - glm::clamp(PosA.y, PosB.y - AABBB.HalfExtents.y, PosB.y + AABBB.HalfExtents.y);
	/* consider it in collision if the distance to that point is less than the circle's radius*/
	return (distX * distX) + (distY * distY) < Circle.Radius * Circle.Radius;
}

bool CheckCircleCircleCol(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB)
{
	assert(ShapeA.Type == ShapeType::CIRCLE && "Called CheckCircleCircleCol but a circle was not provided!");
	assert(ShapeB.Type == ShapeType::CIRCLE && "Called CheckCircleCircleCol but a circle was not provided!");
	return CheckCircleCircleCol(PosA, ShapeA.CircleData, PosB, ShapeB.CircleData);
}

bool CheckAABBCol(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB)
{
	assert(ShapeA.Type == ShapeType::AABB && "Called CheckAABBCol but an AABB was not provided!");
	assert(ShapeB.Type == ShapeType::AABB && "Called CheckAABBCol but an AABB was not provided!");
	return CheckAABBCol(PosA, ShapeA.AABBData, PosB, ShapeB.AABBData);
}

bool CheckCircleAABBCol(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB)
{
	assert(ShapeA.Type == ShapeType::CIRCLE && "Called CheckCircleAABBCol but a circle was not provided!");
	assert(ShapeB.Type == ShapeType::AABB && "Called CheclCircleAABBCol but an AABB was not provided!");
	return CheckCircleAABBCol(PosA, ShapeA.CircleData, PosB, ShapeB.AABBData);
}

glm::vec2 DepenetrateCircleCircle(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB, float& Pen)
{
	/* get the distance between the two circles*/
	float Dist = glm::length(PosB - PosA);
	/* add up the sum of the two radii*/
	float Radii = CircleA.Radius + CircleB.Radius;

	/* find the difference and write it to the referenced variable*/
	Pen = Radii - Dist;

	/* return the direction to correct along*/
	return glm::normalize(PosB - PosA);
}

glm::vec2 DepenetrateCircleCircle(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen)
{
	return DepenetrateCircleCircle(PosA, ShapeA.CircleData, PosB, ShapeB.CircleData, Pen);
}

glm::vec2 DepenetrateAABB(const glm::vec2& PosA, const AABB& AABBA, const glm::vec2& PosB, const AABB& AABBB, float& Pen)
{
	glm::vec2 CenterA = PosA + glm::vec2(AABBA.HalfExtents);
	glm::vec2 CenterB = PosB + glm::vec2(AABBB.HalfExtents);

	glm::vec2 Delta = CenterA - CenterB;

	float OverlappingX = (AABBA.HalfExtents.x + AABBB.HalfExtents.x) - std::abs(PosA.x - PosB.x);
	float OverlappingY = (AABBA.HalfExtents.y + AABBB.HalfExtents.y) - std::abs(PosA.y - PosB.y);


	glm::vec2 Normal(0, 0);

	if (OverlappingX > OverlappingY)
	{
		Pen = OverlappingX;
		// Using a terenary operator to evaluate if true or false;
		Normal.x = (Delta.x > 0.0f) ? 1.0f : -1.0f;
	}
	else
	{
		Pen = OverlappingY;
		// Using a terenary operator to evaluate if true or false;
		Normal.y = (Delta.y > 0.0f) ? -1.0f : 1.0f;
	}

	// returning the Collision Normal
	return Normal;
}

glm::vec2 DepenetrateAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen)
{
	return DepenetrateAABB(PosA, ShapeA.AABBData, PosB, ShapeB.AABBData, Pen);
}

glm::vec2 DepenetrateCircleAABB(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& AABB, float& pen)
{
	// a depenetration vector between a circle center and the closest AABB point.
	glm::vec2 depen;
	// Assigning default penetration values just to be safe
	float penX = 10;
	float penY = 10;
	float penXA = 10;
	float penXB = 10;
	float penYA = 10;
	float penYB = 10;

	// Getting the closest point 
	// Pos a is the circle center, pos b is the aabb center
	// clamping the circle's position to the AABB corners
	float closestPointX = glm::clamp(PosA.x, PosB.x - (AABB.HalfExtents.x), PosB.x + (AABB.HalfExtents.x));
	float closestPointY = glm::clamp(PosA.y, PosB.y - (AABB.HalfExtents.y), PosB.y + (AABB.HalfExtents.y));
	glm::vec2 closestPos = glm::vec2(closestPointX, closestPointY);

	glm::vec2 Difference = PosA - depen;
	float Distance = glm::length(Difference);

	penX = PosA.x + Circle.Radius - closestPos.x;
	penY = PosA.y - Circle.Radius - closestPos.y;
	// output penetration depth
	pen = fminf(glm::abs(penX), glm::abs(penY));

	// y-axis penetration
	penYA = (PosA.y + Circle.Radius) - closestPos.y;
	penYB = (PosA.y - Circle.Radius) - closestPos.y;
	// x-axis penetration
	penXA = (PosA.x + Circle.Radius) - closestPos.x;
	penXB = (PosA.x - Circle.Radius) - closestPos.x;
	// The smallest penetration on each axis
	penX = fminf(glm::abs(penXA), glm::abs(penXB));
	penY = fminf(glm::abs(penYA), glm::abs(penYB));

	// output penetration depth
	pen = fminf(glm::abs(penX), glm::abs(penY));
	/*std::cout << " Penetration: " << pen << std::endl;*/

	// If the circle center is inside the AABB
	if (Distance <= 1.0f)
		return glm::vec2(0, 1);

	// If the circle center is outside the AABB
	if (Distance > 1.0f)
	{
		// Normalizing the push direction, pushing the circle away from the AABB shape
		return glm::normalize(closestPos - PosA);
	}
}


glm::vec2 DepenetrateCircleAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen)
{
	return DepenetrateCircleAABB(PosA, ShapeA.CircleData, PosB, ShapeB.AABBData, Pen);
}
