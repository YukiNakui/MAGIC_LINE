#pragma once
#include "Engine/GameObject.h"

class PlayerCamera :
    public GameObject
{
public:
	PlayerCamera(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	XMVECTOR lookTarget_;
};

