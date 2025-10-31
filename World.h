#pragma once
#include "PhysObject.h"
#include "vector"


class World
{
private:
	// Vector of all our PhysObjects we want to tick and draw
	std::vector<class PhysObject> PhysObjects;
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

	// Signals whether the world needs to shut down

	// Return true if needs shutdown, otherwise false
	bool ShouldClose() const;

	// Signals whether the world needs to perform a fixed tick, return true if needs TickFixed, otherwise false
	bool ShouldTickFixed() const;

protected:
	// Runs at end of Init
	virtual void OnInit() 
	{
		// Construct a PhysObject outside
		PhysObject NewObject;
		NewObject.Position = { 300, 300 };
		PhysObjects.push_back(NewObject);
	}
	// Runs at end of Tick
	virtual void OnTick() {}
	// Runs at end of TickFixed
	virtual void OnTickFixed() {}
	// Runs at end of Draw
	virtual void OnDraw() {}
	// Runs at start of Exit
	virtual void OnExit() {}
};