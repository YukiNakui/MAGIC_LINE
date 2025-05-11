#pragma once
#include "../Engine/GameObject.h"
#include "CDTimer.h"

class CountdownBase : public GameObject {
public:
    CountdownBase(GameObject* parent, const std::string& name);
    virtual ~CountdownBase();

    virtual void Initialize() override = 0;
    virtual void Update() override = 0;
    virtual void Draw() override = 0;
    virtual void Release() override;

protected:
    CDTimer* cdTimer_;
    float deltaTime_;       //フレーム間の時間差
    float elapsedTime_;     //経過時間
    bool isDisplay_;        //表示状態フラグ
    float scale_;           //現在のスケール

public:
    void SetDisplay(bool display) { isDisplay_ = display; }
};

