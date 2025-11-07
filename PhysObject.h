#pragma once

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include "Shapes.h"

class PhysObject
{
	glm::vec2 Forces;
public:
	glm::vec2 Position; // Todo aling with UE; class this "Location"
	glm::vec2 Velocity;

	float ObjectMass;
	bool gravityIsActive;

	// Shape variable as a member, kind of the collision volume
	Shape shapeChoice;

	PhysObject();

	void TickPhys(float DeltaTime);
	void Draw() const;

	// Allows forces to be added to phys object
	// 2-dimensional vector param is for the direction and the length of the force applied
	void AddForce(glm::vec2 Force);

	// Terry has a reference to vec2, vec2&
	void AddAccel(glm::vec2 Accel);

	void AddVelocity(glm::vec2 Velocity);

	void AddImpulse(glm::vec2 Impulse);

	void GravityActive(bool Active);

	/* Calculates an impulse to object A and B, sdduming they are in collision*/
	float ResolveCollision(const glm::vec2& PosA, const glm::vec2& VelA, float MassA,
		const glm::vec2& PosB, const glm::vec2& VelB, float MassB,
		float Elasticity, const glm::vec2& Normal);

	/* Resolves a collision between two PhysObject instances*/
	void ResolvePhysObjects(PhysObject& Lhs, PhysObject& Rhs, float Elasticity, const glm::vec2& Normal, float Pen);
	
};