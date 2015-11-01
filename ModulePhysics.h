#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

	//Utilities
	void AngularImpulse(int degrees);
	void Force(int force_x, int force_y, int point_x, int point_y);

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener 
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type);
	PhysBody* CreatePolygon(int x, int y, int width, int height, int* points, int size, b2BodyType type);

	void CreateRevoluteJoint(PhysBody* body_a, PhysBody* body_b, int anchor_a_x, int anchor_a_y, int anchor_b_x, int anchor_b_y, int min_angle = -360, int max_angle = 360);
	void CreatePrismaticJoint(PhysBody* body_a, PhysBody* body_b, int anchor_x_a, int anchor_y_a, int anchor_x_b, int anchor_y_b, int limit_low, int limit_up);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* mouse_clicked;

	p2List<PhysBody*> body_list; //Save all bodies to destroy them on clean up
};