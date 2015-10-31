#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"


struct SDL_Texture;

struct Entity
{
	SDL_Texture* texture;
	PhysBody*	 body;
	uint fx;

	Entity() : texture(NULL), body(NULL)
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

public:
	
	Entity flipper_right;
	//Entity flipper_left; TODO: just do it

	PhysBody* flipper_circle_right;
	//PhysBody* flipper_circle_left;



};