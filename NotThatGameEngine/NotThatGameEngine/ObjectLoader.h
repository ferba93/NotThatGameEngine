#ifndef __OBJECTLOADER_H__
#define __OBJECTLOADER_H__

#include "Globals.h"
#include "OpenGLFuncionality.h"

class GameObject;
class Application;

	GameObject* LoadModel(Application* app, const char* path, const char* buffer = nullptr, uint size = 0, GameObject* parent = nullptr, bool enabled = true);
	uint LoadTexture(Application* app, const char* path, const char* buffer = nullptr, uint size = 0);

	bool LoadScene(Application* App, const char* buffer, uint size, GameObject* newObject, const char* path);

#endif