#pragma once
#include "../Engine/GameObject.h"
#include"../CDTimer.h"

class CountStart :
    public GameObject
{
    static constexpr int COUNT_IMAGES = 3;

    int hPictCount_[COUNT_IMAGES];
    int hPictStart_;
    int hCountSound_;
    int hStartSound_;
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

    float startElapsedTime_;
    bool isStartVisible_;

    bool isDisplay_;
public:
    void SetDisplay(bool display) { isDisplay_ = display; }
};

