#ifndef __LOAD_H__
#define __LOAD_H__

#include "Globals.h"
#include "JsonManager.h"

class Component;
class Application;
class Mesh;
class Material;
class GameObject;

namespace DataLoading {

	void LoadScene(Application* App, char* buffer);
	void LoadGameObject(Application* App, JSON_Array* gameObjectsArray, int index);
	void LoadModel(Application* App, GameObject* gameObject);
	void LoadMesh(char* fileBuffer, Mesh* mesh);
	void LoadMaterial(Application* App, char* fileBuffer, Material* material);
	uint LoadTexture(Application* app, const char* path, const char* buffer = nullptr, uint size = 0);
	
	struct ComponentReader {

		int componentType;
		long long int componentId;

	};

}

#endif