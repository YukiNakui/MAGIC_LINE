#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"

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
	XMFLOAT3 lookTarget_;
	CDTimer* cdTimer_;
};

