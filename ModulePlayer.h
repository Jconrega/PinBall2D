#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"


struct SDL_Texture;

struct Entity
{
	SDL_Texture*	texture;
	PhysBody*		body;
	PhysBody*		anchor;
	uint fx;

	Entity() : texture(NULL), body(NULL), anchor(NULL)
	{}

};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	void Draw();
	bool CleanUp();

private:
	
	void CreateMap();
	void DrawScore();
	void RespawnBall();

public:
	
	Entity ball;
	iPoint ball_respawn;
	int lives;

	SDL_Texture* ball_lives;

	Entity flipper_right;
	Entity flipper_left; 
	Entity plunger;

	//Score
	SDL_Texture* numbers;
	int score;
};