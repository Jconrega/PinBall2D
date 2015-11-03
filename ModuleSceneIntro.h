#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

struct Bumper
{
	PhysBody* body;
	int life;
	bool sensor;

	Bumper() : body(NULL), sensor(false)
	{}

	Bumper(PhysBody* _body, int _life = 0, bool _sensor = false) : body(_body), life(_life), sensor(_sensor)
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
	
	Bumper* CreateBumper(PhysBody* _body, p2List<Bumper*>* list, bool isListening = false, Module* _module = NULL, bool sensor = false);
	void DrawBumper(Bumper &bump, SDL_Texture* idle, SDL_Texture* light);
	void DrawSensor(Bumper &bump, SDL_Texture* idle, SDL_Texture* light);
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

	p2List<Bumper*> sensor_list;
	SDL_Texture*	sensor_idle;
	SDL_Texture*	sensor_light;

	Bumper			triangle_right;
	SDL_Texture*	triangle_right_idle;
	SDL_Texture*	triangle_right_light;
	Bumper			triangle_left;
	SDL_Texture*	triangle_left_idle;
	SDL_Texture*	triangle_left_light;

};
