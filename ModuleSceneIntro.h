#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

struct Bumper
{
	PhysBody* body;
	SDL_Texture* idle;
	SDL_Texture* light;
	uint life;
	uint fx;

	Bumper() : body(NULL), idle(NULL), light(NULL)
	{};
};

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	void Draw();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:
	void CreateMap();

public:
	p2List<PhysBody*> circles;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	SDL_Texture* background;

	p2List<Bumper*> bump_list;
	p2List<Bumper*> sensor_list;

	Bumper bumper1; //Start left up and rotate clockwise
	Bumper bumper2;
	Bumper bumper3;
	Bumper bumper4;
	Bumper bumper5;
	Bumper bumper6;

	Bumper light_cercle1;
	Bumper light_cercle2;
	Bumper light_cercle3;
	Bumper light_cercle4;
	Bumper light_cercle5;
	Bumper light_cercle6;
	Bumper light_cercle7;
	Bumper light_cercle8;
	Bumper light_cercle9;
	Bumper light_cercle10;
	Bumper light_cercle11;
	Bumper light_cercle12;
	 

};
