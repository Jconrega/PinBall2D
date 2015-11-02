#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"

#define LIGHT_LIFE 10

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
	background = App->textures->Load("pinball/background_done.png");

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
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(),7, b2_dynamicBody));
	}

	Draw();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::Draw()
{
	int x, y;

	App->renderer->Blit(background, 0, 0);

	//Draw all bumpers
	p2List_item<Bumper*>* item;
	item = bump_list.getFirst();

	while (item != NULL)
	{
		item->data->body->GetPosition(x, y);
		if (item->data->life > 0)
		{
			item->data->life--;
			App->renderer->Blit(item->data->light, x, y);
		}
		else
			App->renderer->Blit(item->data->idle, x, y);
		item = item->next;
	}


	//Draw all circles (DEBUG)
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}
}

void ModuleSceneIntro::CreateMap()
{
	bumper1.light = bumper2.light = bumper3.light = bumper4.light = bumper5.light = bumper6.light = App->textures->Load("pinball/bumper_light.png");
	bumper1.idle = bumper2.idle = bumper3.idle = bumper4.idle = bumper5.idle = bumper6.idle = App->textures->Load("pinball/bumper_idle.png");

	bumper1.fx = bumper2.fx = bumper3.fx = bumper4.fx = bumper5.fx = bumper6.fx = App->audio->LoadFx("pinball/Sounds/bumper.ogg");

	int background[58] = {
		59, 314,
		26, 274,
		27, 149,
		31, 126,
		39, 100,
		52, 76,
		72, 51,
		92, 34,
		109, 23,
		119, 19,
		248, 19,
		277, 31,
		301, 45,
		326, 66,
		346, 90,
		363, 122,
		373, 159,
		375, 183,
		377, 649,
		359, 648,
		359, 280,
		352, 278,
		320, 314,
		320, 384,
		349, 413,
		349, 647,
		21, 647,
		20, 416,
		58, 382
	};

	App->physics->CreateChain(0, 0, background, 58, b2_staticBody);

	int metal_l[6] = {
		42, 541,
		41, 574,
		124, 574
	};
	App->physics->CreateChain(0, 0, metal_l, 6, b2_staticBody);

	int metal_r[6] = {
		321, 573,
		320, 540,
		241, 574
	};
	App->physics->CreateChain(0, 0, metal_r, 6, b2_staticBody);

	int wall_l[12] = {
		40, 440,
		41, 507,
		111, 539,
		114, 533,
		48, 501,
		48, 439
	};
	App->physics->CreateChain(0, 0, wall_l, 12, b2_staticBody);

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
	App->physics->CreateChain(0, 0, wall_r, 18, b2_staticBody);

	int triangle_l[10] = {
		106, 508,
		68, 490,
		67, 438,
		77, 436,
		114, 499
	};
	App->physics->CreateChain(0, 0, triangle_l, 10, b2_staticBody);

	int triangle_r[6] = {
		262, 502,
		300, 487,
		293, 447
	};
	App->physics->CreateChain(0, 0, triangle_r, 6, b2_staticBody);

	int bar_down_l[12] = {
		68, 122,
		68, 94,
		75, 88,
		82, 94,
		82, 121,
		75, 127
	};
	App->physics->CreateChain(0, 0, bar_down_l, 12, b2_staticBody);

	int bar_down_r[12] = {
		287, 94,
		287, 121,
		292, 126,
		299, 120,
		300, 93,
		293, 87
	};
	App->physics->CreateChain(0, 0, bar_down_r, 12, b2_staticBody);

	int bar_up_l[12] = {
		101, 67,
		109, 61,
		115, 66,
		115, 93,
		108, 99,
		101, 93
	};
	App->physics->CreateChain(0, 0, bar_up_l, 12, b2_staticBody);

	int bar_up_r[12] = {
		261, 100,
		267, 93,
		267, 67,
		259, 61,
		254, 69,
		254, 95
	};
	App->physics->CreateChain(0, 0, bar_up_r, 12, b2_staticBody);

	bumper1.body = App->physics->CreateCircle(291, 195, 16, b2_staticBody);
	bumper1.body->listener = this;
	bumper2.body = App->physics->CreateCircle(139, 267, 16, b2_staticBody);
	bumper2.body->listener = this;
	bumper3.body = App->physics->CreateCircle(230, 267, 16, b2_staticBody);
	bumper3.body->listener = this;
	bumper4.body = App->physics->CreateCircle(230, 133, 16, b2_staticBody);
	bumper4.body->listener = this;
	bumper5.body = App->physics->CreateCircle(139, 133, 16, b2_staticBody);
	bumper5.body->listener = this;
	bumper6.body = App->physics->CreateCircle(77, 195, 16, b2_staticBody);
	bumper6.body->listener = this;

	bump_list.add(&bumper1);
	bump_list.add(&bumper2);
	bump_list.add(&bumper3);
	bump_list.add(&bumper4);
	bump_list.add(&bumper5);
	bump_list.add(&bumper6);

	
}


void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
	p2List_item<Bumper*>* item;
	item = bump_list.getFirst();

	while (item != NULL)
	{
		if (item->data->body == bodyA)
		{
			item->data->life = LIGHT_LIFE;
			App->audio->PlayFx(item->data->fx);
			//App->player->score += 10; TODO: ADD score
			return;
		}
		item = item->next;
	}

}
