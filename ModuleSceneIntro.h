#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

struct Bumper
{
	PhysBody* body;
	uint life;

	Bumper() : body(NULL)
	{}

	Bumper(PhysBody* _body, uint _life = 0) : body(_body), life(_life)
	{}
};

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
	
	Bumper* CreateBumper(PhysBody* _body, p2List<Bumper*>* list, bool isListening = false, Module* _module = NULL, uint _life = 0);

public:
	p2List<PhysBody*> circles;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	SDL_Texture* background;

	p2List<Bumper*>	bump_list;
	SDL_Texture*	bump_idle;
	SDL_Texture*	bump_light;
	uint			bump_fx;

	p2List<Bumper*> bar_list;
	SDL_Texture*	bar_idle;
	SDL_Texture*	bar_light;


	

};
