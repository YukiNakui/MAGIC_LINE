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
    float elapsedTime_;  //�o�ߎ���
    bool isCounting_;    //�J�E���g�_�E������
    bool finished_;      //�J�E���g�_�E�����I�������

    float scale_;        //���݂̃X�P�[��
    const float maxScale_ = 3.0f; //�J�n���̃X�P�[��
    const float minScale_ = 1.0f; //�ʏ�T�C�Y

    float startElapsedTime_;
    bool isStartVisible_;
};

