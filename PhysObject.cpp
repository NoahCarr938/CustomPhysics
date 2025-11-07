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

float PhysObject::ResolveCollision(const glm::vec2& PosA, const glm::vec2& VelA, float MassA, const glm::vec2& PosB, const glm::vec2& VelB, float MassB, float Elasticity, const glm::vec2& Normal)
{
	/* calculate the relative velocity*/
	glm::vec2 RelVel = VelB - VelA;

	/* Calculate the impulse magnitude*/
	float ImpulseMag = glm::dot(-(1.0f + Elasticity) * RelVel, Normal) /
		glm::dot(Normal, Normal * (1 / MassA + 1 / MassB));

	/* return impulse to apply to both objects*/
	return ImpulseMag;
}

void PhysObject::ResolvePhysObjects(PhysObject& Lhs, PhysObject& Rhs, float Elasticity, const glm::vec2& Normal, float Pen)
{
	/* calculate resolution impulse*/
	/* normal and pen are passed by reference and will be updated*/
	float ImpulseMag = ResolveCollision(Lhs.Position, Lhs.Velocity, Lhs.ObjectMass, Rhs.Position, Rhs.Velocity, Rhs.ObjectMass, Elasticity, Normal);

	/* depenetrate objects*/
	glm::vec2 Mtv = Normal * Pen;
	Lhs.Position -= Mtv;
	Rhs.Position += Mtv;

	/* TODO: do not bother applying impulses to static/kinematic objects*/

	/* apply impulses to update velocity after collision*/
	glm::vec2 Impulse = Normal * ImpulseMag;
	Lhs.AddImpulse(-Impulse);
	Rhs.AddImpulse(Impulse);
}


