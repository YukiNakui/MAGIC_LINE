#pragma once
#include "Engine/GameObject.h"

class Capsule :
    public GameObject
{
	int hModel_;
public:
	Capsule(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

