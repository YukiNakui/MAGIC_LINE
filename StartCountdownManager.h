#pragma once
#include "Engine/GameObject.h"
#include "CountdownNumber.h"
#include "StartDisplay.h"

class StartCountdownManager : public GameObject {
public:
    StartCountdownManager(GameObject* parent);
    ~StartCountdownManager();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //void StartCountdown(); // �J�E���g�_�E�����J�n

    bool IsFinished() const { return isFinished_; }

private:
    CountdownNumber* countdownNumber_;
    StartDisplay* startDisplay_;
    bool isCounting_;  // ���݃J�E���g�_�E�������ǂ���
    bool isStartActive_; // ���݃X�^�[�g�\�������ǂ���
    bool isFinished_;  // �S�Ă̏������I���������ǂ���
};


