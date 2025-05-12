#pragma once
#include "../Engine/GameObject.h"
#include"../Functions/CDTimer.h"
#include"../UI/CountdownNumber.h"

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

    //�{�[���̈ړ��J�n����
    void BallMoveStart() {
        canMove_ = true; //�{�[���̈ړ�������
        ballPos_ = XMLoadFloat3(&transform_.position_); //���݂̃{�[���̈ʒu
        prevBallPos_ = ballPos_; //�O�̈ʒu�����݂̈ʒu�ŏ�����
    }

private:
    CDTimer* cdTimer_; //�^�C�}�[

    //�{�[���̕����������Ǘ�����x�N�g��
    XMVECTOR moveVec_;     //�{�[���̐i�s�����x�N�g��
    XMVECTOR ballPos_;     //���݂̃{�[���̈ʒu
    XMVECTOR prevBallPos_; //�O��̃{�[���̈ʒu
    XMVECTOR ballVelocity_;//�{�[���̑��x

    float gravity_;        //�{�[���ɍ�p����d��
    bool canMove_;         //�{�[�����ړ��\���ǂ����̃t���O

    CountdownNumber* pCountDownNumber_; //�J�E���g�_�E���̐����\���I�u�W�F�N�g

	void HandleCapsuleCollisions();//�J�v�Z���Ƃ̏Փˏ���
	void HandleCollisionWithCapsule(XMVECTOR distance, XMVECTOR closestPoint, XMVECTOR capsuleDir, float distLength);
	void CheckLowSpeedState();//�ᑬ��Ԃ̃`�F�b�N
};

