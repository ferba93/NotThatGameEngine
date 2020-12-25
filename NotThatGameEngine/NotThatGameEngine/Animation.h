#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

class GameObject;
class Mesh;

struct Channels {

	~Channels();

	std::map<float, float3> positionKeys;
	std::map<float, Quat> rotationKeys;
	std::map<float, float3> scaleKeys;

};

class AnimationData {	// This is the resources, the "animations"

public:

	AnimationData(std::string _name, float _duration, float _ticks, int _channels, bool _playing = false);
	~AnimationData();

public:

	std::string name;
	float duration;
	float ticksPerSecond;
	int channelsAmount;
	std::map<std::string, Channels> channels;
	bool playing;

};

class Animation : public Component {	// This is the component, the "animator"

public:

	Animation(long long int _id, GameObject* _gameObject);
	~Animation();
	void PlayAnimation();

private:

	void UpdateGameObjectsTransform(const AnimationData* data);
	void UpdateGameObjectsTransformRecursively(GameObject* gameObject, const AnimationData* data, int currentFrame);
	void UpdateMesh(GameObject* gameObject);
	float3 GetUpdatedChannelPosition(const Channels* channel, const int currentFrame) const;
	Quat GetUpdatedChannelRotation(const Channels* channel, const int currentFrame) const;
	float3 GetUpdatedChannelScale(const Channels* channel, const int currentFrame) const;
	void AnimateMesh(Mesh* mesh);

public:

	std::vector<AnimationData> animationVec;

private:

	int currentAnimationIndex;
	float time;

};

#endif