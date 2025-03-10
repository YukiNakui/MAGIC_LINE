#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"

class CountStart :
    public GameObject
{
    int hPictCount_[3];
    int hPictStart_;
public:
    CountStart(GameObject* parent);
    ~CountStart();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    bool IsFinished() const { return finished_; }
    bool IsStartVisible() const { return isStartVisible_; }
private:
    CDTimer* cdTimer_;
    float deltaTime_;
    int currentCount_; 
    float elapsedTime_;  //経過時間
    bool isCounting_;    //カウントダウン中か
    bool finished_;      //カウントダウンが終わったか

    float scale_;        //現在のスケール
    const float maxScale_ = 3.0f; //開始時のスケール
    const float minScale_ = 1.0f; //通常サイズ

    float startElapsedTime_;
    bool isStartVisible_;
};

