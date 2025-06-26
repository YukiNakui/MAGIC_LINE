#pragma once
#include "Engine/GameObject.h"
class PlayerAndLine :
    public GameObject
{
	int hModel_;
public:
	PlayerAndLine(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

