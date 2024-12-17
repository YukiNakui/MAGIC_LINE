#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"
class Player :
    public GameObject
{
	int hModel_;
public:
	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	CDTimer* cdTimer_;
	XMFLOAT3 lookTarget_;
	XMVECTOR front_;
};

