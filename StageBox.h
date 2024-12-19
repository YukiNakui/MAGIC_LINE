#pragma once
#include "Engine/GameObject.h"

class StageBox :
    public GameObject
{
	int hModel_;
public:
	StageBox(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

