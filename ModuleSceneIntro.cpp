#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle  = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/ball.png");
	background = App->textures->Load("pinball/background.png");

	CreateMap();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(),16, b2_dynamicBody));
		// TODO 8: Make sure to add yourself as collision callback to the circle you creates
	}

	App->renderer->Blit(background, 0, 0);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}



	//------------------------------------------------------
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CreateMap()
{
	int background[48] = {
		20, 570,
		20, 415,
		57, 380,
		57, 311,
		24, 270,
		27, 138,
		40, 92,
		60, 62,
		80, 42,
		103, 25,
		120, 16,
		247, 18,
		282, 32,
		310, 52,
		331, 72,
		351, 100,
		368, 139,
		374, 160,
		374, 251,
		320, 310,
		319, 380,
		346, 411,
		347, 573,
		18, 575
	};
	background_items.add(App->physics->CreateChain(0, 0, background, 48, b2_staticBody));

	int metal_l[6] = {
		42, 541,
		41, 574,
		124, 574
	};
	background_items.add(App->physics->CreateChain(0, 0, metal_l, 6, b2_staticBody));

	int metal_r[6] = {
		321, 573,
		320, 540,
		241, 574
	};
	background_items.add(App->physics->CreateChain(0, 0, metal_r, 6, b2_staticBody));

	int wall_l[12] = {
		40, 440,
		41, 507,
		111, 539,
		114, 533,
		48, 501,
		48, 439
	};
	background_items.add(App->physics->CreateChain(0, 0, wall_l, 12, b2_staticBody));

	int wall_r[18] = {
		322, 502,
		260, 531,
		259, 537,
		271, 532,
		323, 508,
		329, 505,
		329, 489,
		328, 440,
		322, 442
	};
	background_items.add(App->physics->CreateChain(0, 0, wall_r, 18, b2_staticBody));

	int triangle_l[10] = {
		106, 508,
		68, 490,
		67, 438,
		77, 436,
		114, 499
	};
	background_items.add(App->physics->CreateChain(0, 0, triangle_l, 10, b2_staticBody));

	int triangle_r[6] = {
		262, 502,
		300, 487,
		293, 447
	};
	background_items.add(App->physics->CreateChain(0, 0, triangle_r, 6, b2_staticBody));

	int bar_down_l[12] = {
		68, 122,
		68, 94,
		75, 88,
		82, 94,
		82, 121,
		75, 127
	};
	background_items.add(App->physics->CreateChain(0, 0, bar_down_l, 12, b2_staticBody));

	int bar_down_r[12] = {
		287, 94,
		287, 121,
		292, 126,
		299, 120,
		300, 93,
		293, 87
	};
	background_items.add(App->physics->CreateChain(0, 0, bar_down_r, 12, b2_staticBody));

	int bar_up_l[12] = {
		101, 67,
		109, 61,
		115, 66,
		115, 93,
		108, 99,
		101, 93
	};
	background_items.add(App->physics->CreateChain(0, 0, bar_up_l, 12, b2_staticBody));

	int bar_up_r[12] = {
		261, 100,
		267, 93,
		267, 67,
		259, 61,
		254, 69,
		254, 95
	};
	background_items.add(App->physics->CreateChain(0, 0, bar_up_r, 12, b2_staticBody));

	background_items.add(App->physics->CreateCircle(291, 195, 17, b2_staticBody));
	background_items.add(App->physics->CreateCircle(139, 267, 17, b2_staticBody));
	background_items.add(App->physics->CreateCircle(230, 267, 17, b2_staticBody));
	background_items.add(App->physics->CreateCircle(230, 133, 17, b2_staticBody));
	background_items.add(App->physics->CreateCircle(139, 133, 17, b2_staticBody));
	background_items.add(App->physics->CreateCircle(77, 195, 17, b2_staticBody));

	
}


void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
