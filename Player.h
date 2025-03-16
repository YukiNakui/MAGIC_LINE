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
	float currentLineValue_;//���݈��������̗�
	Capsule* pCapsule_;
	std::vector<Capsule*> capsuleList_; // ��ʏ�ɑ��݂���J�v�Z�����X�g
	std::vector<Capsule*> capsulePool_; // �ė��p�p�̃J�v�Z���v�[��

	CountStart* pCountStart_;
	bool canMove_;

private:
	Capsule* GetCapsuleFromPool();
	void ClearCapsules();
};

