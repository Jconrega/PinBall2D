#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//TODO: Set to NULL all pointers of player
	flipper_circle_right = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	CreateMap();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		flipper_right.body->AngularImpulse(360);
	}


	Draw();

	return UPDATE_CONTINUE;
}

void ModulePlayer::Draw()
{
	int x, y;
	flipper_right.body->GetPosition(x, y);
	App->renderer->Blit(flipper_right.texture, x, y, NULL, 1.0f, flipper_right.body->GetRotation(), 0, 0);
}

void ModulePlayer::CreateMap()
{
	//TODO: Load all textures here
	flipper_right.texture = App->textures->Load("pinball/flipper_right.png");

	//TODO: Create all bodies here
	int flipper_r[12] = {
		1, 7,
		57, 1,
		63, 7,
		62, 13,
		56, 18,
		1, 10
	};

	flipper_right.body = App->physics->CreatePolygon(232, 523, 0, 0, flipper_r, 12, b2_dynamicBody);
	flipper_circle_right = App->physics->CreateCircle(232+20, 523 +22, 2, b2_staticBody);
	App->physics->CreateRevoluteJoint(flipper_right.body, flipper_circle_right, 55, 9, 0, 0, -20, 20);
}



