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
			App->renderer->Blit(bump_light, x, y);
		}
		else
			App->renderer->Blit(bump_idle, x, y);
		item = item->next;
	}
	
	//Draw all sensors
	p2List_item<Bumper*>* item_sensor;
	item_sensor = sensor_list.getFirst();

	while (item_sensor != NULL)
	{
		item_sensor->data->body->GetPosition(x, y);
		if (item_sensor->data->sensor == true)
		{
			item_sensor->data->life--;
			App->renderer->Blit(sensor_light, x, y);
		}
		else
		{
			App->renderer->Blit(sensor_idle, x, y);
		}

		item_sensor = item_sensor->next;
	}

	p2List_item<Bumper*>* item2;
	item2 = bar_list.getFirst();

	while (item2 != NULL)
	{
		item2->data->body->GetPosition(x, y);
		if (item2->data->life > 0)
		{
			item2->data->life--;
			App->renderer->Blit(bar_light, x, y, NULL, 1.0f, item2->data->body->GetRotation());
		}
		else
			App->renderer->Blit(bar_idle, x, y, NULL, 1.0f, item2->data->body->GetRotation());
		item2 = item2->next;
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
	bump_light = App->textures->Load("pinball/bumper_light.png");
	bump_idle = App->textures->Load("pinball/bumper_idle.png");
	bump_fx = App->audio->LoadFx("pinball/Sounds/bumper.ogg");

	bar_light = App->textures->Load("pinball/bar_light.png");
	bar_idle = App->textures->Load("pinball/bar_idle.png");

	sensor_idle = App->textures->Load("pinball/circle_idle.png");
	sensor_light = App->textures->Load("pinball/circle_light.png");

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

	int bar[16] = {
		0, 7,
		4, 1,
		9, 1,
		14, 7,
		14, 33,
		10, 37,
		4, 37,
		0, 32
	};

	CreateBumper(App->physics->CreateChain(68, 89, bar, 16, b2_staticBody), &bar_list, true, this);
	CreateBumper(App->physics->CreateChain(101, 62, bar, 16, b2_staticBody), &bar_list, true, this);
	CreateBumper(App->physics->CreateChain(254, 62, bar, 16, b2_staticBody), &bar_list, true, this);
	CreateBumper(App->physics->CreateChain(286, 89, bar, 16, b2_staticBody), &bar_list, true, this);

	CreateBumper(App->physics->CreateCircle(291, 195, 16, b2_staticBody), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(139, 267, 16, b2_staticBody), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(230, 267, 16, b2_staticBody), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(230, 133, 16, b2_staticBody), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(139, 133, 16, b2_staticBody), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(77, 195, 16, b2_staticBody),  &bump_list, true, this);

	CreateBumper(App->physics->CreateCircle(183, 166, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(89, 61, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(123, 39, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(277, 62, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(185, 232, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(75, 327, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(75, 349, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(75, 370, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(301, 327, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(301, 370, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(301, 349, 7, b2_staticBody, true), &sensor_list, true, this);
	CreateBumper(App->physics->CreateCircle(244, 38, 7, b2_staticBody, true), &sensor_list, true, this);
		
}

Bumper* ModuleSceneIntro::CreateBumper(PhysBody* _body, p2List<Bumper*>* list, bool isListening, Module* _module, uint _life)
{
	Bumper* bump = new Bumper(_body, _life);
	if (isListening)
		bump->body->listener = _module;
	list->add(bump);

	return bump;
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
			App->audio->PlayFx(bump_fx);
			//App->player->score += 10; TODO: ADD score
			return;
		}
		item = item->next;
	}

	p2List_item<Bumper*>* item2;
	item2 = bar_list.getFirst();

	while (item2 != NULL)
	{
		if (item2->data->body == bodyA)
		{
			item2->data->life = LIGHT_LIFE;
			App->audio->PlayFx(bump_fx);
			return;
		}
		item2 = item2->next;
	}

	p2List_item<Bumper*>* item_sensor;
	item_sensor = sensor_list.getFirst();

	while (item_sensor != NULL)
	{
		if (item_sensor->data->body == bodyA)
		{
			item_sensor->data->life = LIGHT_LIFE;
			item_sensor->data->sensor = !item_sensor->data->sensor;
			return;
		}
		item_sensor = item_sensor->next;
	}

}
