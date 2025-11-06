#pragma once

#include <cstdint> // for uint8_t which is basically an unsigned char

#include "glm/vec2.hpp"  // for glm::vec2
#include <unordered_map>

struct Circle // 4 byte object
{
	// default value for Radius
	float Radius = 1.0f;
};

// AABB stands for axis aligned bounding boxes
struct AABB
{
	// Terry chose half extents
	/*float x;
	float y;
	float width;
	float height;*/
	// Setting defaults
	glm::vec2 HalfExtents = glm::vec2(10,10);  // Refers to half the dimensions of a bounding box or cube along each axis. // float * 2 => 4 bytes *2 => 8 bytes
};

// uint8 is used to be specific about the amount of bits in this object
enum class ShapeType : uint8_t
{
	// Shifting by bits to indicate the correct shape
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
/**
* Tests if two circles at their given locations and shape are in collision
* 
* @param PosA the first circle's position 
* @param CircleA the first circle's shape
* @param PosB the second circle's position
* @param CircleB the second circle's shape
* @return True if in collision,  false, if not.
*/
bool CheckCircleCircleCol(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB);
bool CheckAABBCol(const glm::vec2& PosA, const AABB& AABBA, const glm::vec2& PosB, const AABB& AABBB);
bool CheckCircleAABBCol(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& AABB);


/* A version of the function that accepts two shape (that we know are circles) and calls the correct func that actually compares*/
/* wrapper for circle-circle collision*/
bool CheckCircleCircleCol(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB);
/* wrapper for AABB-AABB collision*/
bool CheckAABBCol(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB);
/* wrapper for circle-AABB collision*/
bool CheckCircleAABBCol(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB);