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

    // シャドウマップ描画
    void DrawShadowMap(const XMMATRIX& lightViewProj) override;



    //ボールの移動開始処理
    void BallMoveStart() {
        canMove_ = true; //ボールの移動を許可
        ballPos_ = XMLoadFloat3(&transform_.position_); //現在のボールの位置
        prevBallPos_ = ballPos_; //前の位置を現在の位置で初期化
    }

private:
    CDTimer* cdTimer_; //タイマー
	Torus* pTorus_;  //トーラスオブジェクト

    //ボールの物理挙動を管理するベクトル
    XMVECTOR moveVec_;     //ボールの進行方向ベクトル
    XMVECTOR ballPos_;     //現在のボールの位置
    XMVECTOR prevBallPos_; //前回のボールの位置
    XMVECTOR ballVelocity_;//ボールの速度

	float radius_;         //ボールの半径
    float gravity_;        //ボールに作用する重力
    bool canMove_;         //ボールが移動可能かどうかのフラグ

    CountdownNumber* pCountDownNumber_; //カウントダウンの数字表示オブジェクト

	void HandleCapsuleCollisions();//カプセルとの衝突処理
	void HandleCollisionWithTorus(Torus* torus);//トーラスとの衝突処理
	void HandleCollisionWithCapsule(XMVECTOR distance, XMVECTOR closestPoint, XMVECTOR capsuleDir, float distLength);
	void CheckLowSpeedState();//低速状態のチェック
    //トーラスの中を通り抜けたかをチェック
	bool CheckPassedThroughTorus(Torus* torus);
};

