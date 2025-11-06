#pragma once
#include "PhysObject.h"
#include <vector>
#include <memory>


class World
{
private:
	// Vector of all our PhysObjects we want to tick and draw
	// vector is an array in c++
	//std::vector<std::shared_ptr<class PhysObject>> PhysObjects;
	std::vector<PhysObject> PhysObjects;
protected:
	// Elapsed time since last fixed tick
	float AccumulatedFixedTime;
public:
	// Time between fixed ticks
	float TargetFixedStep;
	float GravityScale;
	bool UsingGravity;

	

	World();

	// For initialization
	void Init();
	// For update logic
	void Tick();
	// For physics update logic
	void TickFixed();
	// For drawing
	void Draw();
	// For clean up and termination
	void Exit();

	void Instantiate(std::shared_ptr<PhysObject> Objects);

	// Runs at end of Tick
	void OnTick();
	// Runs at end of TickFixed
	void OnTickFixed();

	// Signals whether the world needs to shut down

	// Return true if needs shutdown, otherwise false
	bool ShouldClose() const;

	// Signals whether the world needs to perform a fixed tick, return true if needs TickFixed, otherwise false
	bool ShouldTickFixed() const;

protected:
	// Runs at end of Init
	virtual void OnInit() 
	{
	

		// Make objects here, first half is pointer, second half is creating object
		/*std::shared_ptr<PhysObject> Object1 = std::make_shared<PhysObject>();
		Object1->shapeChoice.Type = ShapeType::CIRCLE;
		Object1->shapeChoice.CircleData.Radius = 15.0f;
		Object1->Position = { 300, 300 };

		std::shared_ptr<PhysObject> Object2 = std::make_shared<PhysObject>();
		Object2->shapeChoice.Type = ShapeType::CIRCLE;
		Object2->shapeChoice.CircleData.Radius = 10.0f;
		Object2->Position = { 400, 400 };

		std::shared_ptr<PhysObject> Object3 = std::make_shared<PhysObject>();
		Object3->shapeChoice.Type = ShapeType::CIRCLE;
		Object3->shapeChoice.CircleData.Radius = 20.0f;
		Object3->Position = { 350, 350 };*/
	}
	
	// Runs at end of Draw
	virtual void OnDraw() {}
	// Runs at start of Exit
	virtual void OnExit() {}
};