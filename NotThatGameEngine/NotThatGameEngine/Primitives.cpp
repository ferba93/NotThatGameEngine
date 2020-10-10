#include "Primitives.h"

Primitives::Primitives(PrimitiveEnum _type) : type(_type), size(0), vertices(), id(1/*THIS SHOULD BE PROVISIONAL*/){}


Primitives::Primitives(PrimitiveEnum _type, std::vector<float> _vertices) : type(_type) {

	size = _vertices.size();
	vertices = _vertices;

}

Primitives::~Primitives() {}


int Primitives::SetVertices(std::vector<float> _vertices) {
	size = _vertices.size();
	vertices = _vertices;

	return size;
}


std::vector<float>* Primitives::GetVertices() { return &vertices; }


Cube::Cube() : Primitives(PrimitiveEnum::PRIMITIVE_CUBE){}


Cube::Cube(std::vector<float> _vertices) : Primitives (PrimitiveEnum::PRIMITIVE_CUBE, _vertices){}


Cube::~Cube(){}
