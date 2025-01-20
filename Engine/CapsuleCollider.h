#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
	// Collider�N���X��private�����o�ɃA�N�Z�X�ł���悤�ɂ���
		friend class Collider;

		XMFLOAT3 start_;
		XMFLOAT3 end_;
		float radius_;

public:
	//�R���X�g���N�^�i�����蔻��̍쐬�j
	CapsuleCollider(XMFLOAT3 start, XMFLOAT3 end, float radius);

private:
	//�ڐG����
	bool IsHit(Collider* target) override;
};

