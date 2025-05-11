#pragma once
#include "../Engine/GameObject.h"


class CameraOrbit :
    public GameObject
{
public:
    CameraOrbit(GameObject* parent);
    ~CameraOrbit();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //カメラの周回を設定する
    void SetOrbit(const XMFLOAT3& focusPoint, float radius, float speed);

    //周回を停止する
    void StopOrbit() { isOrbiting_ = false; }

	//周回中かどうかを取得する
	bool IsStopOrbit() { return isOrbiting_; }

    //周回処理を更新する
    void Update(float deltaTime);

private:
    bool isOrbiting_;            //周回中であるかどうか
    XMFLOAT3 focusPoint_;        //カメラの注視点
    float orbitRadius_;          //周回の半径
    float orbitSpeed_;           //周回の速度
    float orbitAngle_;           //現在の周回角度
};

