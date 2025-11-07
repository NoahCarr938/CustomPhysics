#include "Shapes.h"
#include "glm/glm.hpp"

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
	return (distX * distX) + (distY * distY);
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

glm::vec2 DepenetrateCircleAABB(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& AABB, float& Pen)
{
	glm::vec2 depen;
	depen = PosA;

	

	if (PosA.x < PosB.x + AABB.HalfExtents.x)
	{
		depen.x = PosB.x + AABB.HalfExtents.x;
	}
	else if (PosA.x > PosB.x + AABB.HalfExtents.x)
	{
		depen.x = PosB.x + AABB.HalfExtents.x;
	}

	if (PosA.y < PosB.y + AABB.HalfExtents.y)
	{
		depen.y = PosB.y + AABB.HalfExtents.y;
	}
	else if (PosA.y > PosB.y + AABB.HalfExtents.y)
	{
		depen.y = PosB.y + AABB.HalfExtents.y;
	}

	glm::vec2 Dist = PosA - depen;
	float Distance = glm::sqrt((Dist.x * Dist.x) + (Dist.y * Dist.y));

	Pen = Circle.Radius - Distance;

	return glm::normalize(depen - PosA);
}

glm::vec2 DepenetrateCircleAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen)
{
	return DepenetrateCircleAABB(PosA, ShapeA.CircleData, PosB, ShapeB.AABBData, Pen);
}
