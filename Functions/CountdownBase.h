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
    float deltaTime_;       //�t���[���Ԃ̎��ԍ�
    float elapsedTime_;     //�o�ߎ���
    bool isDisplay_;        //�\����ԃt���O
    float scale_;           //���݂̃X�P�[��

public:
    void SetDisplay(bool display) { isDisplay_ = display; }
};

