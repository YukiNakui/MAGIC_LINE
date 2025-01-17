#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
	// Collider�N���X��private�����o�ɃA�N�Z�X�ł���悤�ɂ���
		friend class Collider;

public:
	//�R���X�g���N�^�i�����蔻��̍쐬�j
	//�����FbasePos	�����蔻��̒��S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	//�����Fradius	�����蔻��̃T�C�Y�i���a�j
	CapsuleCollider(XMFLOAT3 center, float radius);

private:
	//�ڐG����
	//�����Ftarget	����̓����蔻��
	//�ߒl�F�ڐG���Ă��true
	bool IsHit(Collider* target) override;
};

