#pragma once
#include "Engine/GameObject.h"

class StageFrame :
    public GameObject
{
	int hModel1_;
	int hModel2_;
	int hModel3_;
	Transform loco1_;
	Transform loco2_;
	Transform loco3_;
public:
	StageFrame(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

