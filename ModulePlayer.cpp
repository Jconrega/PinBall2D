#include "Globals.h"
#include "p2DynArray.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

#define MAX_LIFES 5

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_respawn.x = 367;
	ball_respawn.y = 493;
	lifes = MAX_LIFES;

	turn_on_barrier = turn_off_barrier = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	score = 0;

	numbers = App->textures->Load("pinball/numbers.png");

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
	int x, y;
	ball.body->GetPosition(x, y);
	if (y > 600)
		RespawnBall();

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		flipper_right.body->AngularImpulse(360);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		flipper_left.body->AngularImpulse(-360);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		plunger.body->Force(0, -500, 0, 0);
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_DOWN && App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT)
		{
			plunger.body->body->SetGravityScale(-1);
		}
		else
			plunger.body->body->SetGravityScale(1);
	}
	for (int i = 0; i < lifes; i++)
	{
		App->renderer->Blit(ball_lifes, 510 - (20 * i), 127);
	}

	Draw();
	DrawScore();
	
	return UPDATE_CONTINUE;
}

void ModulePlayer::Draw()
{
	int x, y;

	ball.body->GetPosition(x, y);
	App->renderer->Blit(ball.texture, x, y, NULL, 1.0f, ball.body->GetRotation());

	flipper_right.body->GetPosition(x, y);
	App->renderer->Blit(flipper_right.texture, x, y, NULL, 1.0f, flipper_right.body->GetRotation(), 0, 0);

	flipper_left.body->GetPosition(x, y);
	App->renderer->Blit(flipper_left.texture, x, y, NULL, 1.0f, flipper_left.body->GetRotation(), 0, 0);

	plunger.body->GetPosition(x, y);
	App->renderer->Blit(plunger.texture, x, y, NULL, 1.0F, plunger.body->GetRotation(), 0, 0);

	App->renderer->Blit(plunger_top, 359, 500);
	
	barrier.body->GetPosition(x, y);
	App->renderer->Blit(barrier.texture, x, y, NULL, 1.0F, barrier.body->GetRotation(), 0, 0);

}

void ModulePlayer::CreateMap()
{
	//TODO: Load all textures here
	ball.texture = App->textures->Load("pinball/ball.png");
	flipper_right.texture = App->textures->Load("pinball/flipper_right.png");
	flipper_left.texture = App->textures->Load("pinball/flipper_left.png");
	plunger.texture = App->textures->Load("pinball/plunger.png");
	plunger_top = App->textures->Load("pinball/plunger_top.png");
	ball_lifes = ball.texture;
	barrier.texture = App->textures->Load("pinball/barrier.png");
	//TODO: Create all bodies here

	ball.body = App->physics->CreateCircle(367, 493, 7, b2_dynamicBody);
	ball.body->listener = this;
	ball.body->body->SetBullet(true);

	int flipper_r[12] = {
		1, 7,
		57, 1,
		63, 7,
		62, 13,
		56, 18,
		1, 10
	};

	flipper_right.body = App->physics->CreatePolygon(232, 523, 0, 0, flipper_r, 12, b2_dynamicBody);
	flipper_right.anchor = App->physics->CreateCircle(232+20, 523 +22, 2, b2_staticBody);
	App->physics->CreateRevoluteJoint(flipper_right.body, flipper_right.anchor, 55, 9, 0, 0, -20, 20);

	int flipper_l[12] = {
		63, 14,
		10, 18,
		2, 14,
		1, 7,
		8, 1,
		63, 9
	};

	flipper_left.body = App->physics->CreatePolygon(122, 522, 0, 0, flipper_l, 12, b2_dynamicBody);
	flipper_left.anchor = App->physics->CreateCircle(121, 544, 2, b2_staticBody);
	App->physics->CreateRevoluteJoint(flipper_left.body, flipper_left.anchor, 9, 11, 0, 0, -20, 20);

	plunger.body = App->physics->CreateRectangle(366, 539, 14, 78, b2_dynamicBody);
	plunger.anchor = App->physics->CreateRectangle(355, 507, 5, 5, b2_staticBody);
	App->physics->CreatePrismaticJoint(plunger.body, plunger.anchor, 0, 0, 12, 0, -80, -40);

	//Barrier
	turn_on_barrier = App->physics->CreateRectangleSensor(365,224,18,23);
	turn_on_barrier->listener = this;
	turn_off_barrier = App->physics->CreateRectangleSensor(368, 480, 15, 74);
	turn_off_barrier->listener = this;

	int _barrier[8] = {
		0, 20,
		4, 23,
		21, 5,
		16, 1
	};
	barrier.body = App->physics->CreatePolygon(-30, 258, 0, 0, _barrier, 8, b2_staticBody);

}

void ModulePlayer::RespawnBall()
{
	b2Vec2 position(PIXEL_TO_METERS(ball_respawn.x), PIXEL_TO_METERS(ball_respawn.y));
	ball.body->body->SetTransform(position, 0);

	if (lifes - 1 < 0)
	{
		lifes = MAX_LIFES;
		score = 0;
	}
	else
		lifes--;
}

void ModulePlayer::DrawScore()
{
	SDL_Rect rect;
	rect.w = 15;
	rect.h = 20;
	rect.y = 0;

	//Put the score in an array to identify each number
	p2DynArray<int> arr;
	int num = score;
	while (num > 0)
	{
		int result = num % 10;
		arr.PushBack(result);
		num = num / 10;
	}

	//Draw a number of 10 digits
	for (int i = 0; i < 9; i++)
	{
		//Pick the number or fill it with 0
		if (arr.Count() > i)
			rect.x = arr[i] * 15;
		else
			rect.x = 0;

		App->renderer->Blit(numbers, 545-i* 15, 59, &rect, 1.0f, 0, 0, 0);
	}
	
	
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == turn_off_barrier)
	{
		b2Vec2 pos(PIXEL_TO_METERS(-358), PIXEL_TO_METERS(258));
		barrier.body->body->SetTransform(pos, barrier.body->GetRotation());
		return;
	}

	if (bodyA == turn_on_barrier)
	{
		b2Vec2 pos(PIXEL_TO_METERS(358), PIXEL_TO_METERS(258));
		barrier.body->body->SetTransform(pos, barrier.body->GetRotation());
		return;
	}
}

