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
	XMVECTOR moveVec_;//ボールの進行方向ベクトル
	XMVECTOR ballPos_;//現在のボールの位置
	XMVECTOR prevBallPos_;//前のボールの位置
	XMVECTOR ballVelocity_;//ボールの速度
	float gravity_;//重力
	bool canMove_;

	float lowSpeedTime_;                //低速状態の経過時間
	float lowSpeedThreshold_;     //低速と判定する速度
	float timeBeforeSceneChange_; //低速状態が続いたら切り替えまでの時間
};

