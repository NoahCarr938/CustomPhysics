#include <memory>

// [SMART POINTERS]

// unique pointers => SINGLE ownership of an object
// shared pointers => MULTIPLE owners of an object
// weak pointers => NO OWNERSHIP to object

#include "World.h"
#include "PhysObject.h"
#include "raylib.h"

int main(void)
{
	auto App = std::make_unique<World>();

	App->Init();

	//PhysObject TestInstance;

	while (!App->ShouldClose())
	{
		App->Tick();
		while (App->ShouldTickFixed())
		{
			App->TickFixed();
		}
		App->Draw();

		//DrawCircle(300, 300, 5.0f, RED);
	}

	App->Exit();

	return 0;
}