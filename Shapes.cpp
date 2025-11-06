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
