#include "Globals.h"
#include "p2DynArray.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

#define MAX_LIVES 5

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_respawn.x = 367;
	ball_respawn.y = 470;
	lives = MAX_LIVES;
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
		plunger.body->Force(0, -600, 0, 0);
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_DOWN && App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT)
		{
			plunger.body->Force(0, -400, 0, 0);
		}
	}
	for (int i = 0; i < lives; i++)
	{
		App->renderer->Blit(ball_lives, 510 - (20 * i), 127);
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

}

void ModulePlayer::CreateMap()
{
	//TODO: Load all textures here
	ball.texture = App->textures->Load("pinball/ball.png");
	flipper_right.texture = App->textures->Load("pinball/flipper_right.png");
	flipper_left.texture = App->textures->Load("pinball/flipper_left.png");
	plunger.texture = App->textures->Load("pinball/plunger.png");
	ball_lives = ball.texture;
	//TODO: Create all bodies here

	ball.body = App->physics->CreateCircle(367, 470, 7, b2_dynamicBody);
	ball.body->listener = this;

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

	plunger.body = App->physics->CreateRectangle(366, 600, 14, 77, b2_dynamicBody);
	plunger.anchor = App->physics->CreateRectangle(355, 507, 5, 5, b2_staticBody);
	App->physics->CreatePrismaticJoint(plunger.body, plunger.anchor, 0, 0, 12, 0, -80, -40);

}

void ModulePlayer::RespawnBall()
{
	b2Vec2 position(PIXEL_TO_METERS(ball_respawn.x), PIXEL_TO_METERS(ball_respawn.y));
	ball.body->body->SetTransform(position, ball.body->GetRotation());

	if (lives-1 < 0)
		lives = MAX_LIVES;
	else
		lives--;
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

