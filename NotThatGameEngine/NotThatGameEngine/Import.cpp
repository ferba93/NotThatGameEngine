#include "OpenGLFuncionality.h"
#include "Application.h"
#include "Import.h"
#include "EditorScene.h"
#include "ManagerExternal.h"
#include "GameObject.h"
#include "ManagerResource.h"
#include "Save.h"
#include "Load.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Animation.h"

std::string Importer::ImportNewModel(Application* App, const char* path, const char* buffer, uint size) {

	std::string originalName, finalPath;

	App->externalManager->SplitFilePath(path, nullptr, &originalName);
	GameObject* newObject = new GameObject(App, App->idGenerator.Int(), originalName, nullptr, true);

	if (Importer::ImportNewModelComponents(App, buffer, size, newObject, path)) { finalPath = DataSaving::SaveModel(App, newObject, originalName); }
	DeleteWithAllChilds(App, newObject);

	return finalPath;

}


bool Importer::ImportNewModelComponents(Application* App, const char* buffer, uint size, GameObject* newObject, const char* path) {

	aiScene* scene = (aiScene*)aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
	aiMatrix4x4 trans;

	if (scene == nullptr || scene->HasMeshes() == false) {
		LOG("Error loading scene % s.\n", path);
		return false;
	}

	trans = scene->mRootNode->mTransformation;
	std::map< GameObject*, std::vector<int>> meshMap;

	if (scene->mRootNode->mNumChildren != 0) {

		for (uint i = 0; i < scene->mRootNode->mNumChildren; i++) { ImportNodes(App, scene->mRootNode->mChildren[i], newObject, &meshMap, trans); }
		Importer::ImportNewModelMesh(App, scene, &meshMap);
		ImportAnimation(App, scene, newObject);

	}


	aiReleaseImport(scene);

	LOG("Scene with path %s loaded.\n", path);

	return true;
}


void Importer::ImportNodes(Application* App, aiNode* node, GameObject* parent, std::map<GameObject*, std::vector<int>>* meshMap, aiMatrix4x4 accTransform) {

	aiVector3D aiPosition, aiScale;
	aiQuaternion aiRotation;

	node->mTransformation.Decompose(aiScale, aiRotation, aiPosition);

	float3 position(aiPosition.x, aiPosition.y, aiPosition.z);
	float3 scale(aiScale.x, aiScale.y, aiScale.z);
	Quat rotation(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);

	std::string nodeName = node->mName.C_Str();
	bool dummyFound = true;
	while (dummyFound) {

		dummyFound = false;

		if (nodeName.find("_$AssimpFbx$_") != std::string::npos && node->mNumChildren == 1) {

			node = node->mChildren[0];

			node->mTransformation.Decompose(aiScale, aiRotation, aiPosition);
			position += float3(aiPosition.x, aiPosition.y, aiPosition.z);
			scale = float3(scale.x * aiScale.x, scale.y * aiScale.y, scale.z * aiScale.z);
			rotation = rotation * Quat(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);

			nodeName = node->mName.C_Str();
			dummyFound = true;

		}
	}

	long long int id = App->idGenerator.Int();
	GameObject* newObject = new GameObject(App, id, nodeName, parent);
	parent->childs.push_back(newObject);

	aiMatrix4x4 transform(aiVector3D(scale.x, scale.y, scale.z), aiQuaternion(rotation.w, rotation.x, rotation.y, rotation.z), aiVector3D(position.x, position.y, position.z));
	Importer::aiTransformTofloat4x4Transform(transform, newObject->transform);

	std::vector<int> meshVec;

	for (uint i = 0; i < node->mNumMeshes; i++) { meshVec.push_back(node->mMeshes[i]); }

	meshMap->insert(std::pair<GameObject*, std::vector<int>>(newObject, meshVec));

	for (int i = 0; i < node->mNumChildren; i++) { ImportNodes(App, node->mChildren[i], newObject, meshMap, transform); }

}


void Importer::ImportNewModelMesh(Application* App, aiScene* scene, std::map<GameObject*, std::vector<int>>* meshMap) {

	Mesh* mesh = nullptr;
	GameObject* gameObjectPtr = nullptr;

	for (std::map<GameObject*, std::vector<int>>::iterator mapIt = meshMap->begin(); mapIt != meshMap->end(); mapIt++) {

		for (int i = 0; i < mapIt->second.size(); i++) {

			if (i > 0) {

				std::string stringAux = std::to_string(i);
				gameObjectPtr = new GameObject(App, App->idGenerator.Int(), mapIt->first->name + stringAux, mapIt->first->parent);
				gameObjectPtr->transform->SetPosition(mapIt->first->transform->GetPosition());
				gameObjectPtr->transform->SetRotation(mapIt->first->transform->GetEulerQuat());
				gameObjectPtr->transform->SetScale(mapIt->first->transform->GetScale());
				mapIt->first->parent->childs.push_back(gameObjectPtr);
				mesh = (Mesh*)gameObjectPtr->AddComponent(COMPONENT_TYPE::MESH);

			}

			else {

				gameObjectPtr = mapIt->first;
				mesh = (Mesh*)gameObjectPtr->AddComponent(COMPONENT_TYPE::MESH);

			}

			const aiMesh* paiMesh = (aiMesh*)scene->mMeshes[mapIt->second[i]];
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

			mesh->vertexSize = paiMesh->mNumVertices * 3;
			mesh->vertices = new float[mesh->vertexSize];
			mesh->indexSize = paiMesh->mNumFaces * 3;
			if (mesh->indexSize > 0) { mesh->indices = new GLuint[mesh->indexSize]; }
			mesh->normalsSize = paiMesh->mNumVertices * 3;
			mesh->normals = new float[mesh->normalsSize];
			mesh->textureCoordSize = paiMesh->mNumVertices * 2;
			mesh->textureCoord = new float[mesh->textureCoordSize];

			for (unsigned int j = 0; j < paiMesh->mNumVertices; j++) {		// Vertices
				const aiVector3D* pPos = &(paiMesh->mVertices[j]);
				const aiVector3D* pNormal = &(paiMesh->mNormals[j]); //: &Zero3D	// There are the same normals as there are vertices, so we don't need a loop for them
				const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][j]) : &Zero3D;	// Same as above

				mesh->vertices[j * 3] = pPos->x;
				mesh->vertices[j * 3 + 1] = pPos->y;
				mesh->vertices[j * 3 + 2] = pPos->z;
				mesh->textureCoord[j * 2] = pTexCoord->x;
				mesh->textureCoord[j * 2 + 1] = pTexCoord->y;
				mesh->normals[j * 3] = pNormal->x;
				mesh->normals[j * 3 + 1] = pNormal->y;
				mesh->normals[j * 3 + 2] = pNormal->z;
			}

			LOG("New mesh with %i vertices.\n", mesh->vertexSize);

			for (unsigned int j = 0; j < paiMesh->mNumFaces; j++) {		// Indices
				const aiFace& Face = paiMesh->mFaces[j];
				if (Face.mNumIndices != 3) { LOG("Not all faces of %s are triangles.\n", scene->mMeshes[mapIt->second[i]]->mName.C_Str()); }
				mesh->indices[j * 3] = Face.mIndices[0];
				mesh->indices[j * 3 + 1] = Face.mIndices[1];
				mesh->indices[j * 3 + 2] = Face.mIndices[2];
			}

			if (paiMesh->HasBones()) {

				mesh->boneIDs = new int[paiMesh->mNumVertices * 4];
				mesh->boneWeights = new float[paiMesh->mNumVertices * 4];
				mesh->boneDisplayVec = new bool[paiMesh->mNumBones];
				mesh->boneWeightsSize = mesh->boneIDsSize = paiMesh->mNumVertices * 4;
				mesh->boneDisplayVecSize = paiMesh->mNumBones;

				for (int j = 0; j < paiMesh->mNumVertices * 4; j++) { mesh->boneIDs[j] = -1; }
				for (int j = 0; j < paiMesh->mNumVertices * 4; j++) { mesh->boneWeights[j] = 0.0f; }

				for (int j = 0; j < paiMesh->mNumBones; j++) {

					Transform auxTransform(0, nullptr);
					mesh->boneNamesVec.push_back(paiMesh->mBones[j]->mName.C_Str());
					mesh->boneOffsetMatrixVec.push_back(aiTransformTofloat4x4Transform(paiMesh->mBones[j]->mOffsetMatrix, &auxTransform));
					mesh->boneDisplayVec[j] = false;

					for (int weights = 0; weights < paiMesh->mBones[j]->mNumWeights; weights++) {

						int vertexId = paiMesh->mBones[j]->mWeights[weights].mVertexId * 4;

						for (int it = 0; it < 4; it++) {

							if (mesh->boneIDs[vertexId + it] == -1) {

								mesh->boneIDs[vertexId + it] = j;
								mesh->boneWeights[vertexId + it] = paiMesh->mBones[j]->mWeights[weights].mWeight;
								it = 4;

							}

						}

					}

				}

			}

			Importer::ImportNewModelMaterial(App, scene, gameObjectPtr, scene->mMeshes[mapIt->second[i]]->mMaterialIndex);

		}

	}

}


void Importer::ImportNewModelMaterial(Application* App, aiScene* scene, GameObject* newObject, int materialId) {

	Material* material = nullptr;

	if (scene->mNumMaterials > 0) {

		const aiMaterial* pMaterial = scene->mMaterials[materialId];
		aiString Path;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

				std::string name;
				App->externalManager->SplitFilePath(Path.C_Str(), nullptr, &name);
				if (name.size() > 2) {	// Weird bug don't ask
					material = (Material*)newObject->AddComponent(COMPONENT_TYPE::MATERIAL);
					material->SetTextureName(App, name);
					LOG("Material with texture = %s loaded.\n", material->GetTextureName().c_str());
				}
			}

		}

		else if (scene->mNumMaterials < 2) { LOG("This model's texture is not specified as Diffuse."); }

	}

	else { LOG("This model has no materials attached.\n"); }

}


void Importer::ImportAnimation(Application* App, aiScene* scene, GameObject* newObject) {

	if (scene->HasAnimations()) {

		newObject->AddComponent(COMPONENT_TYPE::ANIMATION);
		std::vector<AnimationData>* modelAnimation = &newObject->animation->animationVec;

		for (uint i = 0; i < scene->mNumAnimations; i++) {

			aiAnimation* a = scene->mAnimations[i];
			modelAnimation->push_back(AnimationData(a->mName.C_Str(), a->mDuration, a->mTicksPerSecond));

			for (uint j = 0; j < a->mNumChannels; j++) {

				aiNodeAnim* n = a->mChannels[j];

				std::string channelName = n->mNodeName.C_Str();
				if (channelName.find("_$AssimpFbx$_") != std::string::npos) { channelName = channelName.substr(0, channelName.find("_$AssimpFbx$_")); }
				modelAnimation->at(i).channels.insert(std::pair<std::string, Channels>(channelName, Channels()));

				for (uint p = 0; p < n->mNumPositionKeys; p++) {

					aiVectorKey pk = n->mPositionKeys[p];
					modelAnimation->at(i).channels.find(channelName.c_str())->second.positionKeys.insert(std::pair<float, float3>(pk.mTime, float3(pk.mValue.x, pk.mValue.y, pk.mValue.z)));
				}

				for (uint r = 0; r < n->mNumRotationKeys; r++) {

					aiQuatKey rk = n->mRotationKeys[r];
					modelAnimation->at(i).channels.find(channelName.c_str())->second.rotationKeys.insert(std::pair<float, Quat>(rk.mTime, Quat(rk.mValue.x, rk.mValue.y, rk.mValue.z, rk.mValue.w)));

				}

				for (uint s = 0; s < n->mNumScalingKeys; s++) {

					aiVectorKey sk = n->mScalingKeys[s];
					modelAnimation->at(i).channels.find(channelName.c_str())->second.scaleKeys.insert(std::pair<float, float3>(sk.mTime, float3(sk.mValue.x, sk.mValue.y, sk.mValue.z)));

				}

			}

		}

	}

}


float4x4 Importer::aiTransformTofloat4x4Transform(aiMatrix4x4 matrix, Transform* transform) {

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	matrix.Decompose(scale, rotation, position);

	transform->SetPosition(float3(position.x, position.y, position.z));
	transform->SetRotation(Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	transform->SetScale(float3(scale.x, scale.y, scale.z));	// TODO: I can optimize this by calling directly the recalculation matrix, but with a function that takes position, rotation and scale as values to recalculate it with

	return transform->transform;

}


std::string Importer::ImportTexture(Application* App, std::string fileName, const char* buffer, uint size) {

	std::string finalPath;
	uint imageTest;
	ilGenImages(1, &imageTest);
	ilBindImage(imageTest);

	ILboolean ret = ilLoadL(IL_TYPE_UNKNOWN, buffer, size);

	if (ret == IL_TRUE) {

		finalPath = DataSaving::SaveTexture(App, fileName);
		LOG("Texture %s loaded.\n", fileName.c_str());

	}

	else { LOG("Texture with path %s failed to load.\n", fileName.c_str()); }

	ilDeleteImages(1, &imageTest);

	return finalPath;

}


void Importer::DeleteWithAllChilds(Application* App, GameObject* gameObject) {

	if (gameObject->material != nullptr) { gameObject->material->SetTextureName(App, std::string()); }
	for (uint i = 0; i < gameObject->childs.size(); i++) { DeleteWithAllChilds(App, gameObject->childs[i]); }
	delete gameObject;

}
