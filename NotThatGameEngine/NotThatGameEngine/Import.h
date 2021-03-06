#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Globals.h"
#include "Assimp/include/matrix4x4.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include <map>

class Application;
class GameObject;
class Component;
class Mesh;
class Transform;
struct aiNode;
struct aiScene;

namespace Importer {

	std::string ImportNewModel(Application* app, const char* path, const char* buffer, uint size);
	void ImportNodes(Application* App, aiNode* node, GameObject* parent, std::map<GameObject*, std::vector<int>>* meshMap, aiMatrix4x4 accTransform);
	bool ImportNewModelComponents(Application* App, const char* buffer, uint size, GameObject* newObject, const char* path);
	void ImportNewModelMesh(Application* App, aiScene* scene, std::map<GameObject*, std::vector<int>>* meshMap);
	void ImportNewModelMaterial(Application* App, aiScene* scene, GameObject* newObject, int materialId);
	void ImportAnimation(Application* App, aiScene* scene, GameObject* newObject);

	std::string ImportTexture(Application* App, std::string fileName, const char* buffer, uint size);

	float4x4 aiTransformTofloat4x4Transform(aiMatrix4x4 matrix, Transform* transform);
	void DeleteWithAllChilds(Application* App, GameObject* gameObject);

}

#endif
