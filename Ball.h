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
	XMVECTOR moveVec_;//ボールの進行方向ベクトル
	XMVECTOR ballPos_;//現在のボールの位置
	XMVECTOR prevBallPos_;//前のボールの位置
	XMVECTOR ballVelocity_;//ボールの速度
	float gravity_;//重力
	bool canMove_;
};

