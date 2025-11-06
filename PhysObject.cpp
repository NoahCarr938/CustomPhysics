#include "PhysObject.h"
#include "World.h"
#include "raylib.h"

const float TargetFixedTimestep = 1 / 30.0f;

// Provide safe default values
PhysObject::PhysObject() :
	Position({ 0, 0, }),
	Velocity({ 0, 0 }),
	Forces({ 0, 0 }),
	ObjectMass(10),
	shapeChoice({ShapeType::NONE})
{
}

void PhysObject::TickPhys(float DeltaTime)
{
	
	Velocity += Forces * DeltaTime;

	// zeroing out forces
	Forces = {};

	// Integrate velocity into position
	Position += Velocity * DeltaTime;
}

void PhysObject::Draw() const
{
	switch (shapeChoice.Type)
	{
	case ShapeType::NONE:
		// We will just draw shapeless colliders as a point
		DrawPixel(Position.x, Position.y, RED);
		break;
	case ShapeType::CIRCLE:
		// Draw Circle
		DrawCircleLines(Position.x, Position.y, shapeChoice.CircleData.Radius, RED);
		break;
	case ShapeType::AABB:
		// draw AABB
		DrawRectangleLines(Position.x - shapeChoice.AABBData.HalfExtents.x, Position.y - shapeChoice.AABBData.HalfExtents.y,
			shapeChoice.AABBData.HalfExtents.x * 2.0f, shapeChoice.AABBData.HalfExtents.y * 2.0f,
			BLUE);
		break;
	default:
		break;
	}
	
}

void PhysObject::AddForce(glm::vec2 Force)
{
	Forces += Force * ObjectMass;
	//Forces += Force / ObjectMass;
}

void PhysObject::AddAccel(glm::vec2 Accel)
{
	//Forces += Accel;
	Velocity += Accel * TargetFixedTimestep;
}

void PhysObject::AddVelocity(glm::vec2 Velocity)
{
	//Velocity += ObjectMass;
	Forces += Velocity;
}

void PhysObject::AddImpulse(glm::vec2 Impulse)
{
	Forces += Impulse * ObjectMass;
}

void PhysObject::GravityActive(bool Active)
{
	gravityIsActive = Active;
}


