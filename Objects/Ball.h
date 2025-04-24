#pragma once
#include "../Engine/GameObject.h"
#include"../CDTimer.h"

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
	void BallMoveStart() {
		canMove_ = true; 
		ballPos_ = XMLoadFloat3(&transform_.position_);
		prevBallPos_ = ballPos_;
	}
private:
	CDTimer* cdTimer_;
	XMVECTOR moveVec_;//�{�[���̐i�s�����x�N�g��
	XMVECTOR ballPos_;//���݂̃{�[���̈ʒu
	XMVECTOR prevBallPos_;//�O�̃{�[���̈ʒu
	XMVECTOR ballVelocity_;//�{�[���̑��x
	float gravity_;//�d��
	bool canMove_;

	float lowSpeedTime_;                //�ᑬ��Ԃ̌o�ߎ���
	float lowSpeedThreshold_;     //�ᑬ�Ɣ��肷�鑬�x
	float timeBeforeSceneChange_; //�ᑬ��Ԃ���������؂�ւ��܂ł̎���
};

