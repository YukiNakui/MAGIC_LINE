#pragma once
#include "../Engine/GameObject.h"
#include"../CDTimer.h"
#include"CountStart.h"
#include "../CameraOrbit.h"

class ThemeDisplay :
    public GameObject
{
    int hPict_;
public:
    ThemeDisplay(GameObject* parent);
    ~ThemeDisplay();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    bool IsMoveFinished() const { return hasMoved_; }
private:
    CDTimer* cdTimer_;
    float deltaTime_;
    float elapsedTime_;
    float displayDuration_;
    float transitionDuration_;
    bool isMoving_;
    bool hasMoved_; //ˆê“x‚¾‚¯ˆÚ“®‚·‚é‚½‚ß‚Ìƒtƒ‰ƒO
    Transform targetTrans_;
    float targetScale_;

    CountStart* pCountStart_;
	CameraOrbit* pCameraOrbit_;
};

