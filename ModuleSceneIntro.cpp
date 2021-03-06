#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
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
	background = App->textures->Load("pinball/background_done.png");

	CreateMap();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//Lists
	p2List_item<PhysBody*>* item = circles.getFirst();

	while (item != NULL)
	{
		delete item->data;
		item = item->next;
	}

	circles.clear();

	p2List_item<Bumper*>* item2 = bump_list.getFirst();

	while (item2 != NULL)
	{
		delete item2->data;
		item2 = item2->next;
	}

	bump_list.clear();

	p2List_item<Bumper*>* item3 = bar_list.getFirst();

	while (item3 != NULL)
	{
		delete item3->data;
		item3 = item3->next;
	}

	bar_list.clear();

	p2List_item<Bumper*>* item4 = sensor_list.getFirst();

	while (item4 != NULL)
	{
		delete item4->data;
		item4 = item4->next;
	}

	sensor_list.clear();

	//Texures
	App->textures->Unload(circle);
	App->textures->Unload(background);
	App->textures->Unload(bump_idle);
	App->textures->Unload(bump_light);
	App->textures->Unload(bar_idle);
	App->textures->Unload(bar_light);
	App->textures->Unload(sensor_idle);
	App->textures->Unload(sensor_light);
	App->textures->Unload(triangle_right_idle);
	App->textures->Unload(triangle_left_idle);
	App->textures->Unload(triangle_right_light);
	App->textures->Unload(triangle_left_light);


	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(),7, b2_dynamicBody));
	}

	if (!App->physics->debug)
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
		DrawBumper(*item->data, bump_idle, bump_light);
		item = item->next;
	}

	p2List_item<Bumper*>* item2;
	item2 = bar_list.getFirst();

	while (item2 != NULL)
	{
		DrawBumper(*item2->data, bar_idle, bar_light);
		item2 = item2->next;
	}

	p2List_item<Bumper*>* item3;
	item3 = sensor_list.getFirst();

	while (item3 != NULL)
	{
		DrawSensor(*item3->data, sensor_idle, sensor_light);
		item3 = item3->next;
	}

	DrawBumper(triangle_left, triangle_left_idle, triangle_left_light);
	DrawBumper(triangle_right, triangle_right_idle, triangle_right_light);

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

void ModuleSceneIntro::DrawBumper(Bumper &bump, SDL_Texture* idle, SDL_Texture* light)
{
	int x, y;
	bump.body->GetPosition(x, y);
	int time = bump.life - SDL_GetTicks();
	if (time >= 0)
	{
		App->renderer->Blit(light, x, y);
	}
	else
	{
		App->renderer->Blit(idle, x, y);
		bump.life = 0;
	}
}

void ModuleSceneIntro::DrawSensor(Bumper &bump, SDL_Texture* idle, SDL_Texture* light)
{
	int x, y;
	bump.body->GetPosition(x, y);
	int time = bump.life - SDL_GetTicks();
	if (time < 0)
	{
		bump.life = 0;
	}

	if (bump.sensor)
		App->renderer->Blit(light, x, y);
	else
		App->renderer->Blit(idle, x, y);

}

void ModuleSceneIntro::CreateMap()
{
	bump_light = App->textures->Load("pinball/bumper_light.png");
	bump_idle = App->textures->Load("pinball/bumper_idle.png");
	bump_fx = App->audio->LoadFx("pinball/bumper.ogg");

	bar_light = App->textures->Load("pinball/bar_light.png");
	bar_idle = App->textures->Load("pinball/bar_idle.png");

	triangle_right_idle = App->textures->Load("pinball/triangle_right_idle.png");
	triangle_right_light = App->textures->Load("pinball/triangle_right_light.png");
	triangle_left_light = App->textures->Load("pinball/triangle_left_light.png");
	triangle_left_idle = App->textures->Load("pinball/triangle_left_idle.png");

	sensor_light = App->textures->Load("pinball/sensor_light.png");
	sensor_idle = App->textures->Load("pinball/sensor_idle.png");
	sensor_fx = App->audio->LoadFx("pinball/bonus.ogg");

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

	int triangle_l[20] = {
		0, 52,
		0, 5,
		2, 1,
		6, 0,
		12, 2,
		46, 63,
		47, 68,
		44, 72,
		38, 72,
		3, 56
	};

	
	triangle_left.body = App->physics->CreateChain(67, 435, triangle_l, 20, b2_staticBody);
	triangle_left.body->listener = this;

	int triangle_r[18] = {
		2, 63,
		36, 3,
		40, 1,
		45, 2,
		48, 5,
		48, 53,
		11, 72,
		5, 72,
		1, 67
	};
	triangle_right.body = App->physics->CreateChain(253, 435, triangle_r, 18, b2_staticBody);
	triangle_right.body->listener = this;

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

	//Bar
	CreateBumper(App->physics->CreateChain( 68, 89, bar, 16, b2_staticBody,false, 1.0f), &bar_list, true, this);
	CreateBumper(App->physics->CreateChain(101, 62, bar, 16, b2_staticBody,false, 1.0f), &bar_list, true, this);
	CreateBumper(App->physics->CreateChain(254, 62, bar, 16, b2_staticBody,false, 1.0f), &bar_list, true, this);
	CreateBumper(App->physics->CreateChain(286, 89, bar, 16, b2_staticBody,false, 1.0f), &bar_list, true, this);

	//Circle bumper
	CreateBumper(App->physics->CreateCircle(291, 195, 16, b2_staticBody,false,1.0f), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(139, 267, 16, b2_staticBody,false,1.0f), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(230, 267, 16, b2_staticBody,false,1.0f), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(230, 133, 16, b2_staticBody,false,1.0f), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle(139, 133, 16, b2_staticBody,false,1.0f), &bump_list, true, this);
	CreateBumper(App->physics->CreateCircle( 77, 195, 16, b2_staticBody,false,1.0f),  &bump_list, true, this);

	//Sensor
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

	int sensor_left[8] = {
		81, 441,
		77, 445,
		105, 494,
		110, 492
	};
	App->physics->CreateChain(0, 0, sensor_left, 8, b2_staticBody, false, 1.5f);

	int sensor_right[8] = {
		287, 441,
		291, 445,
		262, 496,
		256, 494
	};
	App->physics->CreateChain(0, 0, sensor_right, 8, b2_staticBody, false, 1.5f);
}

Bumper* ModuleSceneIntro::CreateBumper(PhysBody* _body, p2List<Bumper*>* list, bool isListening, Module* _module, bool _sensor)
{
	Bumper* bump = new Bumper(_body);
	if (isListening)
		bump->body->listener = _module;
	bump->sensor = _sensor;
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
			if (item->data->life == 0)
			{
				item->data->life = SDL_GetTicks() + LIGHT_LIFE;
				App->audio->PlayFx(bump_fx);
				App->player->score += BUMP_CIRCLE_SCORE;
			}
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
			if (item2->data->life == 0)
			{
				item2->data->life = SDL_GetTicks() + LIGHT_LIFE;
				App->audio->PlayFx(bump_fx);
				App->player->score += BUMP_BAR_SCORE;
			}
			return;
		}
		item2 = item2->next;
	}

	p2List_item<Bumper*>* item3;
	item3 = sensor_list.getFirst();

	while (item3 != NULL)
	{
		if (item3->data->body == bodyA)
		{
			if (item3->data->life == 0)
			{
				item3->data->life = SDL_GetTicks() + LIGHT_LIFE;
				item3->data->sensor = !item3->data->sensor;
				App->audio->PlayFx(sensor_fx);
				App->player->score += BUMP_SENSOR_SCORE;
			}
			return;
		}
		item3 = item3->next;
	}

	if (bodyA == triangle_right.body)
	{
		if (triangle_right.life == 0)
		{
			triangle_right.life = SDL_GetTicks() + LIGHT_LIFE;
			App->audio->PlayFx(bump_fx);
			App->player->score += BUMP_TRIANGLE_SCORE;
		}
		return;
	}

	if (bodyA == triangle_left.body)
	{
		if (triangle_left.life == 0)
		{
			triangle_left.life = SDL_GetTicks() + LIGHT_LIFE;
			App->audio->PlayFx(bump_fx);
			App->player->score += BUMP_TRIANGLE_SCORE;
		}
		return;
	}
}
