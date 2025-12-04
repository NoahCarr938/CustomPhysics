#include "World.h"
#include "PhysObject.h"
#include "raylib.h"
#include "glm/vec2.hpp"
#include "Shapes.h"
#include "EnumUtils.h"

#include <unordered_map> // similar to a Dictionary<K,V> Key-Value Collection
#include <vector>
#include <memory>
#include <iostream>

/* the function signature for any collision detection test*/
using CollisionFunc = bool(*)(const glm::vec2&, const Shape&, const glm::vec2&, const Shape&);
/* a map that takes a collision pair and returns the correct function to call*/
using CollisionMap = std::unordered_map<ShapeType, CollisionFunc>;
/* function signature for resolution functions*/
using DepenetrationFunc = glm::vec2(*)(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen);
/* map for associating pairs of collision shapes with their resolution functions*/
using DepenetrationMap = std::unordered_map<ShapeType, DepenetrationFunc>;

CollisionMap ColMap;
/* stores functions for resolving collisions between any two types*/
DepenetrationMap DepenMap;

// Providing safe defaults at 30 fps
World::World() : AccumulatedFixedTime(0), TargetFixedStep(1.0f / 30.0f), GravityScale(2.0f), UsingGravity(true)
{
}

void World::Init()
{

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);

	ColMap[ShapeType::CIRCLE | ShapeType::CIRCLE] = CheckCircleCircleCol;
	ColMap[ShapeType::AABB | ShapeType::AABB] = CheckAABBCol;
	ColMap[ShapeType::CIRCLE | ShapeType::AABB] = CheckCircleAABBCol;

	/* register the handler for Circle-Circle resolutions*/
	DepenMap[ShapeType::CIRCLE | ShapeType::CIRCLE] = DepenetrateCircleCircle;
	/* register the handler for AABB resolutions*/
	DepenMap[ShapeType::AABB | ShapeType::AABB] = DepenetrateAABB;
	/* register the handler for Circle-AABB resolutions*/
	DepenMap[ShapeType::CIRCLE | ShapeType::AABB] = DepenetrateCircleAABB;

	OnInit();
}

void World::Tick()
{
	// Every time it ticks it will increase time passed
	AccumulatedFixedTime += GetFrameTime();

	OnTick();
}

void World::TickFixed()
{
	AccumulatedFixedTime -= TargetFixedStep;

	/* integrate force and velocity*/ 
	for (auto& PObj : PhysObjects)
	{
		if (UsingGravity && PObj.gravityIsActive)
		{
			PObj.AddAccel(glm::vec2{ 0, 9.8f } * GravityScale);
			// integrating velocity into the pos
			PObj.TickPhys(TargetFixedStep);
		}
	}

	for (auto& i : PhysObjects)
	{
		for (auto& j : PhysObjects)
		{
			/*skip if i and j point to the same object*/ 
			if (&i == &j) { continue; }

			/* skip if it doesn't have a collider*/
			if (i.shapeChoice.Type == ShapeType::NONE || j.shapeChoice.Type == ShapeType::NONE) { continue; }

			

			ShapeType ColKey = i.shapeChoice.Type | j.shapeChoice.Type;
			ShapeType Pair = i.shapeChoice.Type | j.shapeChoice.Type;
			auto KeyPairIt = ColMap.find(ColKey);

			bool bHasFunc = KeyPairIt != ColMap.end();

			

			if (bHasFunc)
			{
				/* 'i' stands for Lhs, 'j' stands for Rhs*/
				bool bIsColliding = false;
				/* re-arrange our params if they are out of order*/
				if (i.shapeChoice.Type > j.shapeChoice.Type)
				{
					bIsColliding = ColMap[ColKey](j.Position, j.shapeChoice, i.Position, i.shapeChoice);
				}
				else
				{
					bIsColliding = ColMap[ColKey](i.Position, i.shapeChoice, j.Position, j.shapeChoice);
				}

				ShapeType PairType = i.shapeChoice.Type | j.shapeChoice.Type;

				if (bIsColliding)
				{
					/* Pen will be assigned a value by the depenetration func belowm but it's good to have a default nonetheless*/
					float Pen = 0.0f;
					glm::vec2 Normal = DepenMap[PairType](i.Position, i.shapeChoice,
						j.Position, j.shapeChoice, Pen);


					i.ResolvePhysObjects(
						i,  // first object
						j,   // second object
						1.0f,   // elasticity - hard coded to 1 for now (could be configurable in World)
						Normal, // collision normal
						Pen);   // penetration depth
					//std::cout << "It is colliding" << std::endl;
					//std::cout << "box x pos " << j.Position.x << std::endl;
					//std::cout << "box y pos " << j.Position.y << std::endl;

					
				}
			}

			else
			{
				std::cerr << "Unable to test for collision; no collision function defined for this pair" << std::endl;
			}
		}
	}
	
	// call our lifecycle function
	OnTickFixed();
}

void World::Draw()
{
	BeginDrawing();

	ClearBackground(BLACK);


	// Drawing all objects to screen
	for (auto& const PObj : PhysObjects)
	{
		PObj.Draw();
	}

	OnDraw();

	EndDrawing();
}

void World::Exit()
{
	OnExit();

	CloseWindow(); // close window and OpenGL context
}

//void World::Instantiate(std::shared_ptr<PhysObject> Objects)
//{
//	PhysObjects.push_back(Objects);
//}

bool World::ShouldClose() const
{
	return WindowShouldClose();
}

bool World::ShouldTickFixed() const
{
	return AccumulatedFixedTime >= TargetFixedStep;
}

// Runs at end of Tick
void World::OnTick() 
{
	if (IsMouseButtonPressed(0))
	{
		PhysObject NewObject;
		NewObject.shapeChoice.Type = ShapeType::CIRCLE;
		NewObject.shapeChoice.CircleData.Radius = 25.0f;

		Vector2 CurMousePos = GetMousePosition();
		NewObject.Position.x = CurMousePos.x;
		NewObject.Position.y = CurMousePos.y;

		PhysObjects.push_back(NewObject);
	}
	if (IsMouseButtonPressed(1))
	{
		PhysObject NewObject;
		NewObject.shapeChoice.Type = ShapeType::AABB;
		NewObject.shapeChoice.AABBData.HalfExtents = { 25.0f, 30.0f };

		Vector2 CurMousePos = GetMousePosition();
		NewObject.Position.x = CurMousePos.x;
		NewObject.Position.y = CurMousePos.y;

		PhysObjects.push_back(NewObject);
	};
}

void World::OnTickFixed()
{

}
