#include "Material.h"

Material::Material(int _id, GameObject* _gameObject) : Component(_id, _gameObject, COMPONENT_TYPE::MATERIAL), materialId(0) {}


Material::~Material() {}


void Material::Update() {}


