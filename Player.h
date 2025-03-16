#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"
#include"Capsule.h"
#include"CountStart.h"
#include<vector>

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
	void OnCollision(GameObject* pTarget) override;

private:
	CDTimer* cdTimer_;
	CDTimer* cupsuleTimer_;
	float capsuleSpawnInterval_;
	XMFLOAT3 lookTarget_;
	XMVECTOR front_;

	float maxLineValue_;
	float currentLineValue_;//現在引いた線の量
	Capsule* pCapsule_;
	std::vector<Capsule*> capsuleList_; // 画面上に存在するカプセルリスト
	std::vector<Capsule*> capsulePool_; // 再利用用のカプセルプール

	CountStart* pCountStart_;
	bool canMove_;

private:
	Capsule* GetCapsuleFromPool();
	void ClearCapsules();
};

