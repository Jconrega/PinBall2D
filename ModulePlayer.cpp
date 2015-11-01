#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	ball_texture = App->textures->Load("pinball/ball.png");

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
	App->renderer->Blit(ball_texture, ball->body->GetPosition().x, ball->body->GetPosition().y , NULL, 1.0f, ball->GetRotation());

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		RespawnBall();
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::RespawnBall()
{
	//Create  ball in correct position
	b2Vec2 position;

	position.x = 367;
	position.y = 493;
	ball->body->SetTransform(position, ball->GetRotation());

}



