#include "World.h"
#include "PhysObject.h"

#include <iostream>

#include "raylib.h"
#include <vector>

/* the function signature for any collision detection test*/
using CollisionFunc = bool(*)(const glm::vec2&, const Shape&, const glm::vec2&, const Shape&);
/* a map that takes a collision pair and returns the correct function to call*/
using CollisionMap = std::unordered_map<ShapeType, CollisionFunc>;

// Providing safe defaults at 30 fps
World::World() : AccumulatedFixedTime(0), TargetFixedStep(1.0f / 30.0f), GravityScale(1.0f), UsingGravity(true)
{
	// Default values
	/*TargetFixedStep = 1.0f / 30.0f;
	AccumulatedFixedTime = 0.0f;
	GravityScale = 1.0f;
	usingGravity = true;*/
}

void World::Init()
{
	//CollisionMap[ShapeType::CIRCLE | ShapeType::CIRCLE] = CheckCircleCircleCol;

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);

	// Insert Map Key here

	OnInit();
}

void World::Tick()
{
	// Every time it ticks it will increase time passed
	AccumulatedFixedTime += GetFrameTime();

	//std::cout << "Tick!" << std::endl;
	OnTick();
	
}

void World::TickFixed()
{
	// Apply the gravity in here

	AccumulatedFixedTime -= TargetFixedStep;

	//std::cout << "Fixed Tick!!" << std::endl;
	// call our lifecycle function
	// todo the actual physics update

	


	OnTickFixed();
}

void World::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawText("Congrats!", 190, 200, 20, LIGHTGRAY);

	// Drawing all objects to screen
	for (auto& PObj : PhysObjects)
	{
		PObj.TickPhys(TargetFixedStep);
	}

	OnDraw();

	EndDrawing();
}

void World::Exit()
{
	OnExit();

	CloseWindow(); // close window and OpenGL context
}

bool World::ShouldClose() const
{
	return WindowShouldClose();
}

bool World::ShouldTickFixed() const
{
	return AccumulatedFixedTime >= TargetFixedStep;
}
