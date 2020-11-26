#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Resource.h"

class GameObject;

enum class COMPONENT_TYPE {
	NONE,

	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,

	ERROR_TYPE
};

class Component : public Resource {

public:

	Component(long long int _id, GameObject* _gameObject, COMPONENT_TYPE _type, bool _enabled = true);
	~Component();

	virtual void Enable();
	virtual void Disable();
	virtual void Update();

	void SetComponentDelete();

private:

	ResourceEnum ConvertComponentTypeToResourceType(COMPONENT_TYPE type);

public:

	long long int id;
	bool enabled;
	bool deleteComponent;
	COMPONENT_TYPE type;
	GameObject* owner;

};

#endif