#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"

class Ball :
    public GameObject
{
	int hModel_;
public:
	Ball(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
private:
	CDTimer* cdTimer_;
	XMFLOAT3 speed_;
	bool canMove_;
};

