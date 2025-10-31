#pragma once

#include <glm/vec2.hpp>
#include "Shapes.h"

class PhysObject
{
	glm::vec2 Forces;
public:
	glm::vec2 Position; // Todo aling with UE; class this "Location"
	glm::vec2 Velocity;

	float ObjectMass;

	// Shape variable as a member
	Shape shapeChoice;

	PhysObject();

	void TickPhys(float DeltaTime);
	void Draw() const;

	// Allows forces to be added to phys object
	// 2-dimensional vector param is for the direction and the length of the force applied
	void AddForce(glm::vec2 Force);

	void AddAccel(glm::vec2& Accel);

	void AddVelocity(glm::vec2 Velocity);

	void AddImpulse(glm::vec2 Impulse);

	
};