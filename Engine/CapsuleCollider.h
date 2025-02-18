#pragma once
#include "Collider.h"

//-----------------------------------------------------------
//�J�v�Z���̓����蔻��
//-----------------------------------------------------------
class CapsuleCollider : public Collider
{
	// Collider�N���X��private�����o�ɃA�N�Z�X�ł���悤�ɂ���
		friend class Collider;

		XMFLOAT3 start_;
		XMFLOAT3 end_;
		float radius_;

public:
	//�R���X�g���N�^�i�����蔻��̍쐬�j
	//�����Fcenter	�����蔻��̒��S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	//�����Fstart	�����蔻��̎n�_�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	//�����Fend   	�����蔻��̏I�_�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	//�����Fradius	�����蔻��̃T�C�Y�i���a�j
	CapsuleCollider(XMFLOAT3 center, XMFLOAT3 start, XMFLOAT3 end, float radius);

private:
	//�ڐG����
	//�����Ftarget	����̓����蔻��
	//�ߒl�F�ڐG���Ă��true
	bool IsHit(Collider* target) override;
};

