#pragma once
#include "../Engine/GameObject.h"
#include"../Functions/CDTimer.h"

class Arrow :
    public GameObject
{
	int hModel_;
public:
	Arrow(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
    XMFLOAT3 targetPos_;    //矢印のターゲット位置
    XMFLOAT3 playerPos_;    //プレイヤーの位置
    XMVECTOR directionVec_; //矢印の進行方向ベクトル
    XMVECTOR playerPosVec_; //プレイヤベクトル
    XMVECTOR targetPosVec_; //ターゲットベクトル
    CDTimer* cdTimer_;      //タイマー
    bool isDisplay_;        //矢印の表示状態

public:
    //矢印の表示状態を設定
    void SetDisplay(bool display) { isDisplay_ = display; }
};

