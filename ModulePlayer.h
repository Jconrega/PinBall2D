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
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:
	
	void CreateMap();
	void DrawScore();
	void RespawnBall();

public:
	
	Entity ball;
	iPoint ball_respawn;
	int lifes;

	SDL_Texture* ball_lifes;
	SDL_Texture* plunger_top; //Just for a visual effect

	Entity flipper_right;
	Entity flipper_left; 
	Entity plunger;

	Entity barrier;
	PhysBody* turn_off_barrier;
	PhysBody* turn_on_barrier;

	//Score
	SDL_Texture* numbers;
	int score;
};