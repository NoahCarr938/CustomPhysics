#include "PhysObject.h"
#include "World.h"
#include "raylib.h"

// Provide safe default values
PhysObject::PhysObject() : Position({ 0, 0, }), Velocity({ 0, 0 }), Forces({ 0, 0 }), ObjectMass(10)
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
	//switch (Shape shapeChoice)
	//{
	//case ShapeType::NONE:
	//	// We will just draw shapeless colliders as a point
	//	DrawPixel(Position.x, Position.y, RED);
	//	break;
	//case ShapeType::CIRCLE:
	//	// Draw Circle
	//	DrawCircle(Position.x, Position.y, 5.0f, RED);
	//	break;
	//case ShapeType::AABB:
	//	// draw AABB
	//	break;
	//default:
	//	break;
	//}
	DrawCircle(Position.x, Position.y, 5.0f, RED);
}

void PhysObject::AddForce(glm::vec2 Force)
{
	Forces += Force * ObjectMass;
	//Forces += Force / ObjectMass;
}

void PhysObject::AddAccel(glm::vec2& Accel)
{
	Forces += Accel;
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
}


