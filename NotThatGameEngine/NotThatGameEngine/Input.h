#ifndef __Input_h_
#define __Input_h__

#include "Module.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{
public:
	
	Input(Application* app, bool start_enabled = true);
	~Input();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const;

	KEY_STATE GetMouseButton(int id) const;

	int GetMouseX() const;

	int GetMouseY() const;

	int GetMouseZ() const;

	int GetMouseXMotion() const;

	int GetMouseYMotion() const;

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;
};

#endif