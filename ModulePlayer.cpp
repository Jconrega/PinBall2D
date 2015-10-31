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
	flipper_circle_right = flipper_circle_left = NULL;
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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		flipper_left.body->AngularImpulse(-360);
	}

	Draw();

	return UPDATE_CONTINUE;
}

void ModulePlayer::Draw()
{
	int x, y;

	flipper_right.body->GetPosition(x, y);
	App->renderer->Blit(flipper_right.texture, x, y, NULL, 1.0f, flipper_right.body->GetRotation(), 0, 0);

	flipper_left.body->GetPosition(x, y);
	App->renderer->Blit(flipper_left.texture, x, y, NULL, 1.0f, flipper_left.body->GetRotation(), 0, 0);
}

void ModulePlayer::CreateMap()
{
	//TODO: Load all textures here
	flipper_right.texture = App->textures->Load("pinball/flipper_right.png");
	flipper_left.texture = App->textures->Load("pinball/flipper_left.png");

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

	int flipper_l[12] = {
		63, 14,
		10, 18,
		2, 14,
		1, 7,
		8, 1,
		63, 9
	};

	flipper_left.body = App->physics->CreatePolygon(122, 522, 0, 0, flipper_l, 12, b2_dynamicBody);
	flipper_circle_left = App->physics->CreateCircle(121, 544, 2, b2_staticBody);
	App->physics->CreateRevoluteJoint(flipper_left.body, flipper_circle_left, 9, 11, 0, 0, -20, 20);
}



