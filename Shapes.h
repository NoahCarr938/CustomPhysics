#pragma once

#include <cstdint> // for uint8_t which is basically an unsigned char

#include "glm/vec2.hpp"  // for glm::vec2
#include <unordered_map>

struct Circle
{
	float Radius;
};

struct AABB
{
	// Terry chose half extents
	glm::vec2 HalfExtents;  // Refers to half the dimensions of a bounding box or cube along each axis.
};

enum class ShapeType : uint8_t
{
	NONE   = 0,
	CIRCLE = 1 << 0,
	AABB   = 1 << 1,  /** axis-aligned bounding box (AABB) */ 
};

/* This shape will store info about the shape it holds (through the enum) and the actual data for that shape
through its anonymous union*/
struct Shape
{
	// an enum identifying
	ShapeType Type;

	// add new types of shapes to this anonymous union\
	/*The union is anonymous because it has no name, to ref its memebers, use the "member-of" operator (.) like usual*/
	/*The union needs to able to hold the largest type in its declaration*/ 
	union
	{
		Circle CircleData;
		AABB AABBData;
	};
};

/* Compares two circles and returns true if they are colliding*/
//bool CheckCircleCircleCol(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB);;

/* wrapper for circle-circle collision*/
/* A version of the function that accepts two shape (that we know are circles) and calls the correct func that actually compares*/
//bool CheckCircleCircleCol(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB) 
//{
//	return CheckCircleCircleCol(PosA, ShapeA.CircleData, PosB, ShapeB.CircleData);
//}




