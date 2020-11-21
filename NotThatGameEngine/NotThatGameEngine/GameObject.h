#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class GameObject {

public:

	GameObject(long long int _id, std::string _originalName, std::string _name = "NewGameObject", GameObject* parent = nullptr, bool enabled = true, std::vector<GameObject*> children = std::vector<GameObject*>());
	~GameObject();

	void Update();
	void PostUpdate(uint& defaultTextureId);
	Component* AddComponent(COMPONENT_TYPE _type, long long int _id = 0);
	bool AddGameObjectByParent(GameObject* newObject);
	long long int GenerateComponentId();
	bool CheckChildDeletionById(long long int _id);
	void SetDeleteGameObject();
	Component* FindComponent(COMPONENT_TYPE _type);
	std::vector<Component*> FindComponents(COMPONENT_TYPE _type);
	Component* FindGameObjectChildByComponent(long long int componentId);
	GameObject* FindGameObjectChild(long long int id);

public:

	std::string name;
	const std::string originalName;
	GameObject* parent;
	float4x4 worldTransform;
	std::vector<GameObject*> childs;

	long long int id;
	bool enabled;
	std::vector<Component*> components;

	bool deleteGameObject;

private:

	void CheckGameObjectDeletion();
	void CheckComponentDeletion();

private:

	LCG idGenerator;

};

#endif