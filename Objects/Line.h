#pragma once
#include "../Engine/GameObject.h"

class Line :
    public GameObject
{
	int hModel_;
public:
	Line(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	int GetModelHandle() { return hModel_; }




    // シャドウマップ描画
    void DrawShadowMap(const XMMATRIX& lightViewProj) override;
private:
    float radius_;    //ラインの半径
    XMFLOAT3 start_;  //ラインの始点
    XMFLOAT3 end_;    //ラインの終点
    bool isActive_;   //ラインがアクティブかどうか

public:
    //ラインの半径を取得
    float GetRadius() { return radius_; }
    //ラインの始点を取得
    XMFLOAT3 GetStartPos() { return start_; }
    //ラインの終点を取得
    XMFLOAT3 GetEndPos() { return end_; }
    //ラインのアクティブ状態を設定
    void SetActive(bool active) { isActive_ = active; }
    //ラインがアクティブかどうかを判定
    bool IsActive() const { return isActive_; }
};

