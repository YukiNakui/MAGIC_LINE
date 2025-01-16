#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"
#include"Capsule.h"
#include"Engine/SphereCollider.h"
#include"Engine/Text.h"

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

	float capsuleCDTimer_;//カプセルを出現させる間隔を測るタイマー
	float totalMoveValue_;//総移動距離
	Capsule* pCapsule_;

	Text* pText_;
};

