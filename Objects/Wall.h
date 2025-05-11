#pragma once
#include "../Engine/GameObject.h"
class Wall :
    public GameObject
{
	int hModel_;
public:
	Wall(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

