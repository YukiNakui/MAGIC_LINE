#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"
#include"Capsule.h"

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

	float capsuleCDTimer_;//�J�v�Z�����o��������Ԋu�𑪂�^�C�}�[
	float totalMoveValue_;//���ړ�����
	Capsule* pCapsule_;
};

