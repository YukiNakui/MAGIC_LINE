#pragma once
#include "Engine/GameObject.h"
class Arrow :
    public GameObject
{
	int hArrowPict_;
public:
	Arrow(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

