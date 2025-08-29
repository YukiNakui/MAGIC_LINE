#pragma once
#include "../Engine/GameObject.h"
#include"../Functions/CDTimer.h"
#include"../UI/CountdownNumber.h"
#include"../Torus.h"

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
    void SetColider(XMFLOAT3 scale);

    // �V���h�E�}�b�v�`��
    void DrawShadowMap(const XMMATRIX& lightViewProj) override;



    //�{�[���̈ړ��J�n����
    void BallMoveStart() {
        canMove_ = true; //�{�[���̈ړ�������
        ballPos_ = XMLoadFloat3(&transform_.position_); //���݂̃{�[���̈ʒu
        prevBallPos_ = ballPos_; //�O�̈ʒu�����݂̈ʒu�ŏ�����
    }

private:
    CDTimer* cdTimer_; //�^�C�}�[
	Torus* pTorus_;  //�g�[���X�I�u�W�F�N�g

    //�{�[���̕����������Ǘ�����x�N�g��
    XMVECTOR moveVec_;     //�{�[���̐i�s�����x�N�g��
    XMVECTOR ballPos_;     //���݂̃{�[���̈ʒu
    XMVECTOR prevBallPos_; //�O��̃{�[���̈ʒu
    XMVECTOR ballVelocity_;//�{�[���̑��x

	float radius_;         //�{�[���̔��a
    float gravity_;        //�{�[���ɍ�p����d��
    bool canMove_;         //�{�[�����ړ��\���ǂ����̃t���O

    CountdownNumber* pCountDownNumber_; //�J�E���g�_�E���̐����\���I�u�W�F�N�g

	void HandleCapsuleCollisions();//�J�v�Z���Ƃ̏Փˏ���
	void HandleCollisionWithTorus(Torus* torus);//�g�[���X�Ƃ̏Փˏ���
	void HandleCollisionWithCapsule(XMVECTOR distance, XMVECTOR closestPoint, XMVECTOR capsuleDir, float distLength);
	void CheckLowSpeedState();//�ᑬ��Ԃ̃`�F�b�N
    //�g�[���X�̒���ʂ蔲���������`�F�b�N
	bool CheckPassedThroughTorus(Torus* torus);
};

